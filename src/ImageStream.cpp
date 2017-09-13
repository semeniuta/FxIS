#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>

ImageStream::ImageStream(uint size, uint width, uint height, uint numChannels)
        : stream_size(size), w(width), h(height), num_channels(numChannels), current_index(0) {

    for (int i = 0; i < this->stream_size; i++) {
        cv::Mat im(h, w, CV_8UC(this->num_channels));
        this->images.push_back(im);
    }

}

int ImageStream::storeImageData(unsigned char* imageDataPtr) {

    std::lock_guard<std::mutex> lock(this->mutex);

    cv::Mat im = this->images[current_index];

    memcpy(im.data, imageDataPtr, this->h * this->w * this->num_channels);

    if (this->current_index == this->stream_size - 1) {
        this->current_index = 0;
    } else {
        this->current_index++;
    }

    return 0;

}

int ImageStream::getImage(uint index, cv::Mat out) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (index >= this->stream_size) {
        return -1;
    }

    cv::Mat im = this->images[current_index];
    im.copyTo(out);

    return 0;

}


