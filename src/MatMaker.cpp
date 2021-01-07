#include "MatMaker.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>

void MatMaker::fillMat(cv::Mat& res, unsigned char* imageBuffer) {

    if (this->color) {

        cv::Mat im_bayer(this->height, this->width, CV_8UC1);
        memcpy(im_bayer.data, imageBuffer, this->height * this->width);

        res.create(this->height, this->width, CV_8UC3);
        cv::cvtColor(im_bayer, res, cv::COLOR_BayerBG2BGR);


    } else {

        res.create(this->height, this->width, CV_8UC1);
        memcpy(res.data, imageBuffer, this->height * this->width);

    }

}

MatMaker::MatMaker(std::map<std::string, VmbInt64_t>& features) {

    this->width = (unsigned int)features["Width"];
    this->height = (unsigned int)features["Height"];

    VmbInt64_t pixel_format = features["PixelFormat"];

    switch (pixel_format) {
        case VmbPixelFormatMono8:
            this->color = false;
            break;
        case VmbPixelFormatBayerRG8:
            this->color = true;
            break;
        default:
            throw std::invalid_argument("Pixel format not supported");
    }

}

unsigned int MatMaker::getNumberOfChannels() {
    if (this->color) {
        return 3;
    }
    return 1;
}
