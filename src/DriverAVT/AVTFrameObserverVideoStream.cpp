#include "DriverAVT/AVTFrameObserverVideoStream.h"
#include "DriverAVT/AVTFrameObserverBasic.h"
#include <opencv2/opencv.hpp>
#include <string>

AVTFrameObserverVideoStream::AVTFrameObserverVideoStream(CameraPtr cam, MatMaker mm, std::string windowName)
        : AVTFrameObserverBasic(cam), mat_maker(mm), window_name(windowName) { }

void AVTFrameObserverVideoStream::processFrame(FramePtr frame) {

    VmbErrorType err;

    VmbUchar_t* image_buffer;

    err = frame->GetImage(image_buffer);

    cv::Mat im;
    this->mat_maker.fillMat(im, image_buffer);

    cv::imshow(this->window_name, im);
    cv::waitKey(1);

}