#include "SimpleObserver.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <opencv2/opencv.hpp>

SimpleFrameObserver::SimpleFrameObserver(CameraPtr cameraPointer, MatMaker& mm) : IFrameObserver(cameraPointer), mat_maker(mm) {

    std::cout << "SimpleFrameObserver created" << std::endl;

}

void SimpleFrameObserver::FrameReceived(const FramePtr framePointer) {

    std::cout << "SimpleFrameObserverFrameReceived is invoked" << std::endl;

    VmbFrameStatusType err_recv;
    VmbErrorType err;

    VmbUchar_t* image_buffer;

    if (framePointer->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        if (err_recv == VmbFrameStatusComplete) {

            std::cout << "Receive OK" << std::endl;

            err = framePointer->GetImage(image_buffer);

            cv::Mat im;
            this->mat_maker.fillMat(im, image_buffer);
            cv::imwrite("image_from_cam.jpg", im);

        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);
}


