#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"

#define PI 3.14159265

#define MAX_PARTICLES 10

#define HALF_CAMERA_ANGLE 67 // Axis M1013 is about 67 degrees, but this might be correct
#define ERROR_MARGIN 3
#define RECTANGULARITY_LIMIT 40
#define ASPECT_RATIO_LIMIT 55
#define MINIMUM_AREA 150
#define TAPE_WIDTH_LIMIT 50
#define VERTICAL_SCORE_LIMIT 50
#define LR_SCORE_LIMIT 50

class RobotDemo : public SimpleRobot {
	RobotDrive myRobot;
	Joystick stick;
	
	struct Scores {
		double rectangularity;
		double aspectRatioVertical;
		double aspectRatioHorizontal;
	};
	
	struct Target {
			int verticalIndex;
			int horizontalIndex;
			bool hot;
			double totalScore;
			double leftScore;
			double rightScore;
			double tapeWidthScore;
			double verticalScore;
		};

public:
	RobotDemo():
		myRobot(1, 2),
		stick(1)
	{
		myRobot.SetExpiration(0.1);
		myRobot.SetSafetyEnabled(false);
	}

	void Autonomous() {
		Scores *scores;
		Target target;
		int verticalTargets[MAX_PARTICLES];
		int horizontalTargets[MAX_PARTICLES];
		int numVertTargets, numHorizTargets;
//		AxisCamera &camera = AxisCamera::GetInstance();
		ParticleFilterCriteria2 maskCriteria[] = {{IMAQ_MT_AREA, MINIMUM_AREA, 65535, false, false}};
		
		ColorImage *image;
		image = new RGBImage("/27ft_test_image.jpg");
//		image = camera.GetImage();
		Threshold maskThreshold(150, 195, 0, 255, 40, 255);
		BinaryImage *mask = image -> ThresholdHSV(maskThreshold);
		mask -> Write("/mask.bmp");
		BinaryImage *maskedImage = mask -> ParticleFilter(maskCriteria, 1);
		maskedImage -> Write("/maskedImage.bmp");
		
		vector<ParticleAnalysisReport> *reports = maskedImage->GetOrderedParticleAnalysisReports();
		numVertTargets = numHorizTargets = 0;
		
		
		if (reports -> size() > 0) {
			scores = new Scores[reports -> size()];
			
			for (int i = 0; i < MAX_PARTICLES && i < reports->size(); i++) {
				ParticleAnalysisReport *report = &(reports -> at(i));
				
				scores[i].rectangularity = getRectScore(report);
				scores[i].aspectRatioVertical = getAspectRatioScore(maskedImage, report, true);
				scores[i].aspectRatioHorizontal = getAspectRatioScore(maskedImage, report, false);
				
				if (scores[i].rectangularity > RECTANGULARITY_LIMIT && scores[i].aspectRatioHorizontal > ASPECT_RATIO_LIMIT) {
					horizontalTargets[numHorizTargets++] = i;
				} else if (scores[i].rectangularity > RECTANGULARITY_LIMIT && scores[i].aspectRatioVertical > ASPECT_RATIO_LIMIT) {
					verticalTargets[numVertTargets++] = i;
				}
			}
			
			target.totalScore = target.leftScore = target.rightScore = target.tapeWidthScore = target.verticalScore = 0;
			target.verticalIndex = verticalTargets[0];
			for (int v = 0; v < numVertTargets; v++) {
				ParticleAnalysisReport *verticalReport = &(reports -> at(verticalTargets[v]));
				for (int h = 0; h < numHorizTargets; h++) {
					ParticleAnalysisReport *horizontalReport = &(reports -> at(horizontalTargets[h]));
					double horizWidth, vertWidth, horizHeight;
					imaqMeasureParticle(maskedImage -> GetImaqImage(), horizontalReport -> particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &horizWidth);
					imaqMeasureParticle(maskedImage -> GetImaqImage(), verticalReport -> particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &vertWidth);
					imaqMeasureParticle(maskedImage -> GetImaqImage(), horizontalReport -> particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &horizHeight);
					
					double leftScore = valueToScore(1.2*(verticalReport->boundingRect.left - horizontalReport->center_mass_x)/horizWidth);
					double rightScore = valueToScore(1.2*(horizontalReport->center_mass_x - verticalReport->boundingRect.left - verticalReport->boundingRect.width)/horizWidth);
					double tapeWidthScore = valueToScore(vertWidth/horizHeight);
					double verticalScore = valueToScore(1-(verticalReport->boundingRect.top - horizontalReport->center_mass_y)/(4*horizHeight));
					
					double total = leftScore > rightScore ? leftScore : rightScore;
					total += tapeWidthScore + verticalScore;
					
					if (total > target.totalScore) {
						target.horizontalIndex = horizontalTargets[h];
						target.verticalIndex = verticalTargets[v];
						target.totalScore = total;
						target.leftScore = leftScore;
						target.rightScore = rightScore;
						target.tapeWidthScore = tapeWidthScore;
						target.verticalScore = verticalScore;
					}
				}
				target.hot = isHot(target);
			}
			
			if (numVertTargets > 0) {
				ParticleAnalysisReport *distanceReport = &(reports -> at(target.verticalIndex));
				double distance = getDistance(maskedImage, distanceReport);
				if(target.hot) {
					printf("Hot target located \n");
					printf("Distance: %f \n", distance);
				} else {
					printf("No hot target present \n");
					printf("Distance: %f \n", distance);
				}
			}
		}
		delete maskedImage;
		delete mask;
		delete image;
		delete scores;
		delete reports;
	}
	
	double getRectScore(ParticleAnalysisReport *report){
		if(report -> boundingRect.width * report -> boundingRect.height != 0){
			return 100 * report -> particleArea / (report -> boundingRect.width * report -> boundingRect.height);
		}
		return 0;
	}
	
	double getAspectRatioScore(BinaryImage *image, ParticleAnalysisReport *report, bool vertical){
		double rectLong, rectShort, idealAspectRatio, aspectRatio;
		idealAspectRatio = vertical ? (4.0/32) : (23.5/4);
		
		imaqMeasureParticle(image -> GetImaqImage(), report -> particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
		imaqMeasureParticle(image -> GetImaqImage(), report -> particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);
		
		if(report -> boundingRect.width > report -> boundingRect.height){
			aspectRatio = valueToScore(((rectLong / rectShort) / idealAspectRatio));
		} else {
			aspectRatio = valueToScore(((rectShort / rectLong) / idealAspectRatio));
		}
		return aspectRatio;
	}
	
	double valueToScore(double value) {
		return max(0, min(100, (1 - fabs(1 - value)) * 100));
	}
	
	bool isHot(Target target) {
		bool hot = target.tapeWidthScore >= TAPE_WIDTH_LIMIT;
		hot &= target.verticalScore >= VERTICAL_SCORE_LIMIT;
		hot &= (target.leftScore > LR_SCORE_LIMIT) || (target.rightScore > LR_SCORE_LIMIT);
		
		return hot;
	}
	
	double getDistance (BinaryImage *image, ParticleAnalysisReport *report) {
		double rectLong, height;
		
		imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
		height = min(report -> boundingRect.height, rectLong);
		
		double cameraRadians = HALF_CAMERA_ANGLE * PI / 360;
		return 15360 / (height * 24 * tan(cameraRadians));
	}

	void OperatorControl() {
		// Operator control is for dummies.
	}

	void Test() {
		// Testing is also for dummies.
	}
};

START_ROBOT_CLASS(RobotDemo);
