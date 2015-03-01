#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    namedWindow("Teleop Guide", CV_WINDOW_AUTOSIZE);

    Mat overlay = imread("Guidelines.png", CV_LOAD_IMAGE_UNCHANGED);
    if (!overlay.data) {
        throw std::invalid_argument("Invalid file path!");
    }

    VideoCapture vCap;
    vCap.open("http://10.0.8.4/mjpg/video.mjpg");

    while (true) {
        Mat feed;
        vCap.read(feed);

        for (int y = 0; y < overlay.rows; ++y) {
            for (int x = 0; x < overlay.cols; ++x) {
                double opacity = ((double)overlay.data[y * overlay.step + x * overlay.channels() + 3]) / 255.0;
                for (int i = 0; opacity > 0 && i < feed.channels(); ++i) {
                    unsigned char overlayPx = overlay.data[y * overlay.step + x * overlay.channels() + i];
                    unsigned char feedPx = feed.data[y * feed.step + x * feed.channels() + i];
                    feed.data[y * feed.step + x * feed.channels() + i] = feedPx * (1.0 - opacity) + overlayPx * opacity;
                }
            }
        }

        imshow("Teleop Guide", feed);

        if (waitKey(1) == 27) { // Quit the program when the user presses escape
            break;
        }
    }

    return 0;
}