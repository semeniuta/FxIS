#ifndef MATMAKER_H
#define MATMAKER_H

#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>

class MatMaker {

public:
    explicit MatMaker(std::map<std::string, VmbInt64_t>& features);
    void fillMat(cv::Mat& res, unsigned char* imageBuffer);
    unsigned int getNumberOfChannels();

private:
    bool color;
    unsigned int width;
    unsigned int height;

};

#endif