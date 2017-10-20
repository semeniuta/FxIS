#ifndef CBCALIB_H
#define CBCALIB_H

#include <opencv2/opencv.hpp>

bool findCBC(const cv::Mat& image, cv::Size pattern_size_wh, std::vector<cv::Point2f>& corners);

#endif
