#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <iostream>

ImageStream::ImageStream(uint size, uint width, uint height, uint numChannels)
        : stream_size(size), cvm(size), w(width), h(height), num_channels(numChannels) {

    for (int i = 0; i < this->stream_size; i++) {

        cv::Mat im(h, w, CV_8UC(this->num_channels));
        this->images.push_back(im);
    }

}

int ImageStream::storeImageData(unsigned char* imageDataPtr, std::chrono::high_resolution_clock::time_point t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    memcpy(this->images[this->cvm.getCurrentIndex()].data, imageDataPtr, this->h * this->w * this->num_channels);

    this->cvm.storeTimestamp(t);

    return 0;

}

int ImageStream::getImage(unsigned long index, cv::Mat& out) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (index >= this->stream_size) {
        return -1;
    }

    cv::Mat im = this->images[this->cvm.getCurrentIndex()];
    im.copyTo(out);

    return 0;

}

int ImageStream::getImage(TimePoint t, cv::Mat& out) {

    std::lock_guard<std::mutex> lock(this->mutex);

    unsigned long index_to_get = this->cvm.searchNearestTime(t);

    return this->getImage(index_to_get, out);

}

