#include "VisionAlg/CBCalib.h"

int main() {

    cv::Mat im;

    im = cv::imread("../data/opencv_cb_left04.jpg", cv::IMREAD_GRAYSCALE);

    std::vector<cv::Point2f> corners;

    cv::Size pattern_size_wh(9, 6);

    bool found = findCBC(im, pattern_size_wh, corners);

    cv::drawChessboardCorners(im, pattern_size_wh, corners, found);

    cv::namedWindow("Corners", cv::WINDOW_AUTOSIZE);
    cv::imshow("Corners", im);

    cv::waitKey();

    return 0;
}