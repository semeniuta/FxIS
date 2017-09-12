#include "SimpleObserver.h"
#include <iostream>
#include <cstdio>
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

            //printf("Image pointer address: %x\n", (unsigned long)image_buffer);

            cv::Mat im(h, w, CV_8UC1);

            // memcpy( pCurBitmapBuf, pCurSrc, pBitmap->width * nNumColors );

            int num_colors = 1;
            VmbUchar_t* pixel_ptr = image_buffer;
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    im.at<uchar>(i, j) = *pixel_ptr;
                    pixel_ptr++;
                }
            }

            cv::imwrite("image_from_cam.jpg", im);


        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);
}
