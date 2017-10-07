#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <iostream>

const unsigned int NUM_TIMESTAMPS = 2;

/*
 * 0 - frame arrived
 * 1 - frame got processed (for now: stored)
 *
 * */

ImageStream::ImageStream(uint size) : stream_size(size), ctv(size, NUM_TIMESTAMPS), ready(false) { }

ImageStream::ImageStream(uint size, uint width, uint height, uint numChannels)
        : stream_size(size), ctv(size, NUM_TIMESTAMPS) {

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

int ImageStream::storeImageData(unsigned char* imageDataPtr, TimePoint t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        return -1;
    }

    memcpy(this->images[this->ctv.getCurrentIndex()].data, imageDataPtr, this->h * this->w * this->num_channels);

    this->ctv.storeTimestamp(t, 0);
    this->ctv.storeTimestamp(currentTime(), 1);
    this->ctv.advance();

    this->waiting_for_next_image.notify();

    return 0;

}

int ImageStream::getImage(
        TimePoint t,
        cv::Mat& out,
        TimestampsMatrix& timestampsCopy,
        unsigned long& index,
        unsigned long& current_index,
        std::vector<TimePoint>& timeMeasurements
) {

    this->waiting_for_next_image.wait();

    auto t_request_mutex = currentTime();

    std::lock_guard<std::mutex> lock(this->mutex);

    auto t_got_mutex = currentTime();

    if (!this->ready) {
        return -1;
    }

    unsigned long index_to_get = this->ctv.searchNearestTime(t, 0);

    auto t_done_searching = currentTime();

    cv::Mat im = this->images[index_to_get];
    im.copyTo(out);

    this->ctv.contentSnapshot(timestampsCopy);

    index = index_to_get;
    current_index = this->ctv.getCurrentIndex();

    auto t_done_copying = currentTime();

    timeMeasurements = {t_request_mutex, t_got_mutex, t_done_searching, t_done_copying};

    return 0;

}

