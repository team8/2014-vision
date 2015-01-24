#include <iostream>
#include <queue>
#include <opencv2/opencv.hpp>

#define CAMERA_ANGLE 70 / 360.0 * M_PI // The angle of the camera's FOV in radians
#define TARGET_REAL_HEIGHT 7.0 / 10.0 * 12.0 // The actual height of the target in feet

using namespace cv;
using namespace std;
using Contour = vector<Point>;

// Global variables
queue<double> angles;
double totalAng = 0;

Mat loadImage(string filePath) {
    Mat image;
    image = imread(filePath);
    if (!image.data) {
        throw invalid_argument("Invalid file path!");
    }

    return image;
}

// Masks the image so that only the reflective strips are left
Mat mask(Mat image) {
    // Masks the image and stores it in strips
    Mat strips;
    inRange(image, Scalar(65, 90, 0), Scalar(145, 255, 50), strips);

    // Removes small discrepancies
    int strength = 2;
    erode(strips, strips, getStructuringElement(MORPH_RECT, Size(strength, strength)));
    dilate(strips, strips, getStructuringElement(MORPH_RECT, Size(strength, strength)));
    dilate(strips, strips, getStructuringElement(MORPH_RECT, Size(strength, strength)));
    erode(strips, strips, getStructuringElement(MORPH_RECT, Size(strength, strength)));

    return strips;
}

// Approximate the L strips into hexagons and filter out other noise
vector<Contour> genApproxPolys(vector<Contour> contours) {
    vector<Contour> approxPolys;
    for (Contour contour : contours) {
        Contour approx;
        // Approximates contours into  the fewest points possible while maintaining their shape
        approxPolyDP(Mat(contour), approx, arcLength(Mat(contour), true) * 0.02, true);

//        if (approx.size() == 6) { // Filter out non-hexagons because an L is a hexagon.
            approxPolys.push_back(approx);
//        }
    }
    return approxPolys;
}

// Finds the average height of the two L strips in pixels
double stripHeight(vector<Contour> approxPolys) {
    // The height of nothing is 0
    if (approxPolys.size() == 0) {
        return 0;
    }

    // Find the average height of the two L strips
    int totalHeight = 0;
    for (Contour poly : approxPolys) {
        // Finds the maximum vertical distance between two points of the strips, which is the height of the strip.
        // This compensates for any angle between the camera and the strips.
        int maxVertDiff = 0;
        for (int i = 0; i < poly.size() - 1; i++) {
            int vertDiff = abs(poly[i].y - poly[i+1].y);
            if (vertDiff > maxVertDiff)
                maxVertDiff = vertDiff;
        }
        int vertDiff = abs(poly[0].y - poly[poly.size()-1].y); // Repeat for the first and last points
        if (vertDiff > maxVertDiff)
            maxVertDiff = vertDiff;

        totalHeight += maxVertDiff;
    }

    return totalHeight / approxPolys.size();
}

// Finds the distance to the totes
double getDist(Mat image, vector<Contour> approxPolys) {
    // If no L strips were found, don't try to find the distance.
    if (approxPolys.size() == 0) {
        return -1;
    }

    /* Complex math using the equation from
     * https://wpilib.screenstepslive.com/s/3120/m/8731/l/90361-identifying-and-processing-the-targets#DistanceContinued
     *
     * It uses the number of columns in the image to get the image's width. The image width is being used because we know
     * the camera's horizontal FOV angle, but not the vertical one.
     */
    double dist = TARGET_REAL_HEIGHT * image.cols / (2 * stripHeight(approxPolys) * tan(CAMERA_ANGLE));
    return dist;
}

// Find the distance between two points
double dist(Point a, Point b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// Should find the angle the robot is to the tote
double angleToTote(vector<Contour> approxPolys) {
    // If no L strips were found, don't try to find the angle.
    if (approxPolys.size() == 0) {
        return -1;
    }

    double angleTotal = 0;
    for (Contour poly : approxPolys) {
        // Calculate the height of the L strip, which is the same as the ideal width
        double idealWidth = 0;
        for (int i = 0; i < poly.size() - 1; i++) {
            double vertDiff = dist(poly[i], poly[i+1]);
            if (vertDiff > idealWidth)
                idealWidth = vertDiff;
        }
        double vertDiff = dist(poly[0], poly[poly.size()-1]); // Repeat for the first and last points
        if (vertDiff > idealWidth)
            idealWidth = vertDiff;

        // Calculate the width of the L strip
        Point bottomPoints[] = {Point(-1, -1), Point(-1, -1)};
        for (int i = 0; i < poly.size(); i++) {
            if (poly[i].y > bottomPoints[0].y) {
                bottomPoints[1] = bottomPoints[0];
                bottomPoints[0] = poly[i];
            } else if (poly[i].y > bottomPoints[1].y) {
                bottomPoints[1] = poly[i];
            }
        }
        double width = dist(bottomPoints[0], bottomPoints[1]);

        angleTotal += acos(width / idealWidth);
    }

    return angleTotal / approxPolys.size();
}

Mat processImage(Mat image, double &dist, double &angle) {
    Mat strips = mask(image);

    // Find and store the L strips in contours
    vector<Contour> contours;
    vector<Vec4i> hierarchy; // Needed for drawing the contours
    findContours(strips, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Find the distance to the tote
    vector<Contour> approxPolys = genApproxPolys(contours);
    dist = getDist(image, approxPolys);

    // Keeps a running average of the last 10 angle measurements
    double newAngle = angleToTote(approxPolys);
    if (newAngle != -1 && newAngle == newAngle) { // Makes sure newAngle isn't NaN
        if (totalAng != totalAng) { // If the totalAngle is NaN
            cout << "SADAS" << endl;
            totalAng = 0;
            while (!angles.empty()) angles.pop();
        }

        angles.push(newAngle);
        totalAng += angles.back();
        while (angles.size() > 10) {
            totalAng -= angles.front();
            angles.pop();
        }

        angle = totalAng / angles.size();
    }

    // Draw the outlines of the L strips
    Mat particles = Mat::zeros(strips.size(), CV_8UC3);
    for (int i = 0; i < approxPolys.size(); i++) {
        drawContours(particles, approxPolys, i, Scalar(100, 255, 0), 1, 8, hierarchy, 0, Point());
    }

    return particles;
}

int main() {
    namedWindow("Vision", CV_WINDOW_AUTOSIZE);

    VideoCapture vCap;
    vCap.open("http://10.0.8.4/mjpg/video.mjpg");

    while(true) {
        double dist = 0;
        double angle = 0;

        Mat image;
        vCap.read(image);

        Mat processed = processImage(image, dist, angle);
        putText(processed, "Dist: " + to_string(dist), Point(12, 64), CV_FONT_NORMAL, 2, Scalar(255, 255, 255));
        putText(processed, "Angle: " + to_string(angle / M_PI * 180), Point(12, 128), CV_FONT_NORMAL, 2, Scalar(255, 255, 255));

        imshow("Vision", processed);

        if (waitKey(1) == 27) { // Quit the program when the user presses escape
            break;
        }
    }

    return 0;
}