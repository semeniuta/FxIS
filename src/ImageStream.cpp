#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <iostream>

ImageStream::ImageStream(uint size) : stream_size(size), ctv(size, 1), ready(false) { }

ImageStream::ImageStream(uint size, uint width, uint height, uint numChannels)
        : stream_size(size), ctv(size, 1) {

    this->init(width, height, numChannels);

}

void ImageStream::init(uint width, uint height, uint numChannels) {

    this->w = width;
    this->h = height;
    this->num_channels = numChannels;

    for (int i = 0; i < this->stream_size; i++) {

        cv::Mat im(h, w, CV_8UC(this->num_channels));
        this->images.push_back(im);
    }

    this->ready = true;

}

int ImageStream::storeImageData(unsigned char* imageDataPtr, std::chrono::high_resolution_clock::time_point t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        return -1;
    }

    memcpy(this->images[this->ctv.getCurrentIndex()].data, imageDataPtr, this->h * this->w * this->num_channels);

    this->ctv.storeTimestamp(t, 0);

    this->waiting_for_next_image.notify();

    return 0;

}

int ImageStream::getImage(unsigned long index, cv::Mat& out) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (index >= this->stream_size || !this->ready) {
        return -1;
    }

    cv::Mat im = this->images[this->ctv.getCurrentIndex()];
    im.copyTo(out);

    return 0;

}

int ImageStream::getImage(TimePoint t, cv::Mat& out, TimePoint& tOut) {

    this->waiting_for_next_image.wait();

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        return -1;
    }

    unsigned long index_to_get = this->ctv.searchNearestTime(t, 0);

    cv::Mat im = this->images[this->ctv.getCurrentIndex()];
    im.copyTo(out);

    tOut = this->ctv.getTimestamp(index_to_get, 0);

    return 0;

}

int ImageStream::getImage(TimePoint t, cv::Mat& out, std::vector<std::vector<TimePoint>>& timestamps, unsigned long& index) {

    this->waiting_for_next_image.wait();

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        return -1;
    }

    unsigned long index_to_get = this->ctv.searchNearestTime(t, 0);

    cv::Mat im = this->images[this->ctv.getCurrentIndex()];
    im.copyTo(out);

    this->ctv.contentSnapshot(timestamps);

    index = index_to_get;


    return 0;

}

