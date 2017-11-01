#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <iostream>

/*
 * 0 - frame arrived
 * 1 - frame processed and stored
 *
 * */

ImageStream::ImageStream(uint size)
        : stream_size(size), ctv(size, NUM_TIMESTAMPS_IN_IMAGESTREAM), ready(false) { }

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

void ImageStream::storeImageData(cv::Mat image, TimePoint t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        throw std::runtime_error("ImageStream is not yet ready");
    }

    this->images[this->ctv.getCurrentIndex()] = image;

    this->ctv.storeTimestamp(t, 0);
    this->ctv.storeTimestamp(currentTime(), 1);
    this->ctv.advance();

    this->waiting_for_next_image.notify();

}

void ImageStream::getImage(
        TimePoint t,
        ImageResponse& out
) {

    this->waiting_for_next_image.wait();

    auto t_request_mutex = currentTime();

    std::lock_guard<std::mutex> lock(this->mutex);

    auto t_got_mutex = currentTime();

    if (!this->ready) {
        throw std::runtime_error("ImageStream is not yet ready");
    }

    unsigned long index_to_get = this->ctv.searchNearestTime(t, 0);

    auto t_done_searching = currentTime();

    cv::Mat im = this->images[index_to_get];
    im.copyTo(out.image);

    this->ctv.contentSnapshot(out.timestamps_snapshot);

    out.target_index = index_to_get;
    out.current_index = this->ctv.getCurrentIndex();

    auto t_done_copying = currentTime();

    out.time_measurements = {t_request_mutex, t_got_mutex, t_done_searching, t_done_copying};

}

cv::Size ImageStream::getImageDimension() {

    return cv::Size(this->w, this->h);

}

unsigned int ImageStream::getNumberOfChannels() {

    return this->num_channels;

}

unsigned int ImageStream::getWidth() {
    return this->w;
}

unsigned int ImageStream::getHeight() {
    return this->h;
}

