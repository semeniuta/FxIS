#include "FrameObserverVideoStream.h"
#include <opencv2/opencv.hpp>
#include <string>

FrameObserverVideoStream::FrameObserverVideoStream(CameraPtr cam, MatMaker& mm, std::string windowName)
        : IFrameObserver(cam), mat_maker(mm), window_name(windowName) { }

void FrameObserverVideoStream::FrameReceived(FramePtr framePointer) {

    VmbFrameStatusType err_recv;
    VmbErrorType err;

    VmbUchar_t* image_buffer;

    if (framePointer->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        if (err_recv == VmbFrameStatusComplete) {

            err = framePointer->GetImage(image_buffer);

            cv::Mat im;
            this->mat_maker.fillMat(im, image_buffer);

            cv::imshow(this->window_name, im);
            cv::waitKey(1);

        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);

}