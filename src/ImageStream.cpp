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
        : stream_size(size), ctv_(size, NUM_TIMESTAMPS_IN_IMAGESTREAM), ready_(false) { }

void ImageStream::init(uint width, uint height, uint numChannels) {

    image_width_ = width;
    image_height_ = height;
    num_channels_ = numChannels;

    for (int i = 0; i < stream_size; i++) {

        cv::Mat im(image_height_, image_width_, CV_8UC(num_channels_));
        images_.push_back(im);
    }

    ready_ = true;

}

void ImageStream::storeImageData(cv::Mat image, TimePoint t) {

    std::lock_guard<std::mutex> lock(mutex_);

    if (!ready_) {
        throw std::runtime_error("ImageStream is not yet ready");
    }

    images_[ctv_.getCurrentIndex()] = image;

    ctv_.storeTimestamp(t, 0);
    ctv_.storeTimestamp(currentTime(), 1);
    ctv_.advance();

    waiting_for_next_image_.notify();

}

void ImageStream::getImage(
        TimePoint t,
        ImageResponse& out
) {

    std::cout << "[DEBUG] getImage requested. ci=" << ctv_.getCurrentIndex() << std::endl;

    waiting_for_next_image_.wait();

    auto t_request_mutex = currentTime();

    std::lock_guard<std::mutex> lock(mutex_);

    auto t_got_mutex = currentTime();

    if (!ready_) {
        throw std::runtime_error("ImageStream is not yet ready");
    }

    unsigned long index_to_get = ctv_.searchNearestTime(t, 0);

    std::cout << "[DEBUG] target index found (after new frame is acquired). ci=" << ctv_.getCurrentIndex();
    std::cout << ", target=" << index_to_get << std::endl;

    auto t_done_searching = currentTime();

    cv::Mat im = images_[index_to_get];
    im.copyTo(out.image);

    ctv_.contentSnapshot(out.timestamps_snapshot);

    out.target_index = index_to_get;
    out.current_index = ctv_.getCurrentIndex();

    auto t_done_copying = currentTime();

    out.time_measurements = {t_request_mutex, t_got_mutex, t_done_searching, t_done_copying};

}

cv::Size ImageStream::getImageDimension() {

    return cv::Size(image_width_, image_height_);

}

unsigned int ImageStream::getNumberOfChannels() {

    return num_channels_;

}

unsigned int ImageStream::getWidth() {
    return image_width_;
}

unsigned int ImageStream::getHeight() {
    return image_height_;
}

