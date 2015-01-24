#include <opencv2/opencv.hpp>

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

void drawContours(Mat masked) {
    vector<Contour> contours;
    vector<Vec4i> hierarchy; // Needed for drawing the contours
    findContours(masked, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Contour> approxPolys;
    for (Contour contour : contours) {
        Contour approx;
        // Approximates contours into  the fewest points possible while maintaining their shape
        approxPolyDP(Mat(contour), approx, arcLength(Mat(contour), true) * 0.02, true);

        approxPolys.push_back(approx);
    }

    Mat particles = Mat::zeros(masked.size(), CV_8UC3);
    for (int i = 0; i < approxPolys.size(); i++) {
        drawContours(particles, approxPolys, i, Scalar(100, 255, 0), 1, 8, hierarchy, 0, Point());
    }

    imshow("Contours", particles);
}

int main() {
    namedWindow("Masked Image", CV_WINDOW_AUTOSIZE);
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    namedWindow("Calibration", CV_WINDOW_FREERATIO);

    int minH = 0, maxH = 180;
    int minS = 0, maxS = 255;
    int minV = 0, maxV = 255;

    createTrackbar("Min H", "Calibration", &minH, 180);
    createTrackbar("Max H", "Calibration", &maxH, 180);
    createTrackbar("Min S", "Calibration", &minS, 255);
    createTrackbar("Max S", "Calibration", &maxS, 255);
    createTrackbar("Min V", "Calibration", &minV, 255);
    createTrackbar("Max V", "Calibration", &maxV, 255);

    VideoCapture vCap;
    vCap.open("http://10.0.8.4/mjpg/video.mjpg");

    while(true) {
        Mat image;
        vCap.read(image);

        Mat masked;
        inRange(image, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), masked);

        erode(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
        dilate(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
        dilate(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
        erode(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));

        imshow("Masked Image", masked);

        drawContours(masked);

        if (waitKey(15) == 112) { // If you press the "P" key, it will print the min and max HSV values to the console.
            printf("Scalar(%i, %i, %i), Scalar(%i, %i, %i)\n", minH, minS, minV, maxH, maxS, maxV);
        }

        if (waitKey(30) == 27) {
            break;
        }
    }

    return 0;
}