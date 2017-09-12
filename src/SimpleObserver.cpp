#include "SimpleObserver.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <opencv2/opencv.hpp>

SimpleFrameObserver::SimpleFrameObserver(CameraPtr cameraPointer) : IFrameObserver(cameraPointer) {

    std::cout << "SimpleFrameObserver created" << std::endl;

}

void SimpleFrameObserver::FrameReceived(const FramePtr framePointer) {

    std::cout << "SimpleFrameObserverFrameReceived is invoked" << std::endl;

    VmbFrameStatusType err_recv;
    VmbErrorType err;

    VmbUchar_t* image_buffer;

    if (framePointer->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        VmbUint32_t w;
        VmbUint32_t h;
        VmbPixelFormatType pixel_format;

        if (err_recv == VmbFrameStatusComplete) {
            std::cout << "Receive OK" << std::endl;

            err = framePointer->GetImage(image_buffer);
            err = framePointer->GetWidth(w);
            err = framePointer->GetHeight(h);
            err = framePointer->GetPixelFormat(pixel_format);

            std::cout << "Image size: " << w << " x " << h << std::endl;

            if (pixel_format == VmbPixelFormatMono8) {
                std::cout << "Pixel format: VmbPixelFormatMono8" << std::endl;
            }

            cv::Mat im(h, w, CV_8UC1);
            int num_colors = 1;
            memcpy(im.data, image_buffer, h * w * num_colors);

            cv::Mat im_res(h, w, CV_8UC3);
            cv::cvtColor(im, im_res, CV_BayerBG2BGR);

            cv::imwrite("image_from_cam.jpg", im_res);

        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);
}
