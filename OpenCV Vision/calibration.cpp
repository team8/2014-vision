#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat loadImage(string filePath) {
    Mat image;
    image = imread(filePath);
    if (!image.data) {
        throw invalid_argument("Invalid file path!");
    }

    return image;
}

int main() {
    const int numImages = 12;
    string imageNames[numImages] = {
            "2.5ft_0deg.jpg",
            "2.5ft_30deg.jpg",
            "2.5ft_60deg.jpg",
            "5ft_0deg.jpg",
            "5ft_30deg.jpg",
            "5ft_60deg.jpg",
            "7.5ft_0deg.jpg",
            "7.5ft_30deg.jpg",
            "7.5ft_60deg.jpg",
            "10ft_0deg.jpg",
            "10ft_30deg.jpg",
            "10ft_60deg.jpg"
    };

    Mat images[numImages];
    for (int i = 0; i < numImages; i++) {
        images[i] = loadImage("Test_Images/" + imageNames[i]);
        namedWindow(imageNames[i], CV_WINDOW_AUTOSIZE);
    }

    namedWindow("Calibration", CV_WINDOW_AUTOSIZE);

    int minH = 0, maxH = 150;
    int minS = 25, maxS = 185;
    int minV = 0, maxV = 20;

    createTrackbar("Min H", "Calibration", &minH, 179);
    createTrackbar("Max H", "Calibration", &maxH, 179);
    createTrackbar("Min S", "Calibration", &minS, 255);
    createTrackbar("Max S", "Calibration", &maxS, 255);
    createTrackbar("Min V", "Calibration", &minV, 255);
    createTrackbar("Max V", "Calibration", &maxV, 255);

    while(true) {
        for (int i = 0; i < numImages; i++) {
            Mat masked;
            inRange(images[i], Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), masked);

            erode(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
            dilate(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
            dilate(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));
            erode(masked, masked, getStructuringElement(MORPH_RECT, Size(5, 5)));

            imshow(imageNames[i], masked);
        }

        if (waitKey(30) == 112) { // If you press the "P" key, it will print the min and max HSV values to the console.
            printf("Scalar(%i, %i, %i), Scalar(%i, %i, %i)\n", minH, minS, minV, maxH, maxS, maxV);
        }

        if (waitKey(30) == 27) {
            break;
        }
    }

    return 0;
}