#include <iostream>
#include <opencv2/opencv.hpp>

int getNumberOfChannels(const cv::Mat& im) {
    auto type = (unsigned int) im.type();
    return 1 + (type >> CV_CN_SHIFT);
}

int main() {

    cv::Mat im_gray;
    cv::Mat im_color;

    im_gray = cv::imread("../data/opencv_cb_left04.jpg", cv::IMREAD_GRAYSCALE);
    im_color = cv::imread("../../UntilFocus/data/untilfocus_1/14.png", cv::IMREAD_COLOR);

    std::cout << getNumberOfChannels(im_gray) << std::endl;
    std::cout << getNumberOfChannels(im_color) << std::endl;

    std::cout << im_gray.channels() << std::endl;
    std::cout << im_color.channels() << std::endl;

    cv::namedWindow("im_gray", cv::WINDOW_AUTOSIZE);
    cv::imshow("im_gray", im_gray);

    cv::namedWindow("im_color", cv::WINDOW_AUTOSIZE);
    cv::imshow("im_color", im_color);

    cv::waitKey();

    return 0;

};