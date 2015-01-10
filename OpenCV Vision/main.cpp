#include <iostream>
#include <opencv2/opencv.hpp>

#define CAMERA_ANGLE 67 / 360.0 * M_PI
#define TARGET_REAL_HEIGHT 7.0 / 10.0 * 12.0

using namespace cv;
using namespace std;
using Contour = vector<Point>;

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
    inRange(image, Scalar(60, 110, 0), Scalar(180, 255, 60), strips);

    // Removes small discrepancies
    erode(strips, strips, getStructuringElement(MORPH_RECT, Size(5, 5)));
    dilate(strips, strips, getStructuringElement(MORPH_RECT, Size(5, 5)));
    dilate(strips, strips, getStructuringElement(MORPH_RECT, Size(5, 5)));
    erode(strips, strips, getStructuringElement(MORPH_RECT, Size(5, 5)));

    return strips;
}

// Approximate the L strips into hexagons and filter out other noise
vector<Contour> genApproxPolys(vector<Contour> contours) {
    vector<Contour> approxPolys;
    for (Contour contour : contours) {
        Contour approx;
        // Approximates contours into  the fewest points possible while maintaining their shape
        approxPolyDP(Mat(contour), approx, arcLength(Mat(contour), true) * 0.02, true);

        if (approx.size() == 6) { // Filter out non-hexagons
            approxPolys.push_back(approx);
        }
    }
    return approxPolys;
}

double stripHeight(vector<Contour> approxPolys) {
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
    // Complex math
    double dist = TARGET_REAL_HEIGHT * image.cols / (2 * stripHeight(approxPolys) * tan(CAMERA_ANGLE));
    return dist;
}

// Should find the angle the robot is to the tote
double angleToTote(vector<Contour> approxPolys) {
    // The width of the vertical bar if it is being viewed head on
    double idealWidth = (2.0 * stripHeight(approxPolys)) / 7.0;

    // Finds the actual width of the vertical bar
    int totalWidth = 0;
    for (Contour contour : approxPolys) {
        Point topPoints[] = {contour[0], contour[1]};
        for (int i = 2; i < contour.size(); i++) {
            if (contour[i].y < topPoints[0].y) {
                topPoints[1] = topPoints[0];
                topPoints[0] = contour[i];
            } else if (contour[i].y < topPoints[1].y) {
                topPoints[1] = contour[i];
            }
        }
        totalWidth += abs(topPoints[0].x - topPoints[1].x);
    }

    double avgWidth = totalWidth / approxPolys.size();

    // The angle the robot is to the tote
    return acos(avgWidth / idealWidth);
}

int main() {
    Mat image = loadImage("Test.jpg");
    Mat strips = mask(image);

    // Find and store the L strips in contours
    vector<Contour> contours;
    vector<Vec4i> hierarchy; // Needed for drawing the contours
    findContours(strips, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    // Find the distance to the tote
    vector<Contour> approxPolys = genApproxPolys(contours);
    double dist = getDist(image, approxPolys);
    double angle = angleToTote(approxPolys);
    cout << "Distance to tote: " << dist << endl;
    cout << "Angle to tote: " << angle << endl;

    // Draw the outlines of the L strips
    Mat particles = Mat::zeros(strips.size(), CV_8UC3);
    for (int i = 0; i < approxPolys.size(); i++) {
        drawContours(particles, approxPolys, i, Scalar(100, 255, 0), 1, 8, hierarchy, 0, Point());
    }

    // Display the L strip outlines
    namedWindow("Vision", CV_WINDOW_AUTOSIZE);
    imshow("Vision", particles);

    waitKey(0); // Quit the program on a keypress

    return 0;
}