#include "ImageStream.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

ImageStream::ImageStream(uint size, uint width, uint height, uint numChannels)
        : stream_size(size), w(width), h(height), num_channels(numChannels), current_index(0), first_fill(true) {

    for (int i = 0; i < this->stream_size; i++) {

        cv::Mat im(h, w, CV_8UC(this->num_channels));
        this->images.push_back(im);

        std::chrono::time_point<std::chrono::high_resolution_clock> tp;
        this->timestamps.push_back(tp);

    }

}

int ImageStream::storeImageData(unsigned char* imageDataPtr, std::chrono::high_resolution_clock::time_point t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    memcpy(this->images[current_index].data, imageDataPtr, this->h * this->w * this->num_channels);

    this->timestamps[this->current_index] = t;

    std::cout << this->current_index << std::endl;

    if (this->current_index == this->stream_size - 1) {
        this->current_index = 0;
        if (this->first_fill) {
            this->first_fill = false;
        }
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

int ImageStream::getImage(TimePoint t, cv::Mat out) {

    std::lock_guard<std::mutex> lock(this->mutex);

    // TODO
}

long ImageStream::getInd(ulong i) {

    if (this->first_fill) {

        if (i >= this->current_index) {
           return -1;
        }

        return i;
    }

    if (i < 0 || i >= this->stream_size) {
        return -1;
    }

    if (i <= (this->stream_size - 1 - this->current_index)) {
        // Part of the vector after current_index
        return this->current_index + i;
    } else {
        // Part of the vector before current_index
        return i - this->stream_size + this->current_index;
    }

}

long ImageStream::searchNearestTime(TimePoint t) {

    if (this->timestamps.empty()) {
        return -1;
    }

    return this->searchNearestTime(t, 0, this->stream_size - 1);
}

long ImageStream::searchNearestTime(TimePoint t, ulong indexFrom, ulong indexTo) {

    if (indexFrom < indexTo) {
        return -1;
    }

    if (this->timestamps.size() == 1) {
        if (this->timestamps[getInd(0)] != t) {
            return -1;
        } else {
            return 0;
        }
    }

    if (t < this->timestamps[getInd(0)]) {
        return -1;
    }

    ulong input_size = indexTo - indexFrom + 1;

    if (input_size == 2) {

        std::chrono::duration d1 = t - this->timestamps[getInd(indexFrom)];
        std::chrono::duration d2 = this->timestamps[getInd(indexTo)] - t;

        if (d1 <= d2) {
            return indexFrom;
        }
        return indexTo;

    }

    unsigned long index_middle = this->timestamps.size() / 2;

    if (this->timestamps[getInd(index_middle)] == t) {
        return index_middle;
    }

    if (this->timestamps[getInd(index_middle)] < t) {
        return searchNearestTime(t, indexFrom, index_middle);
    } else {
        return searchNearestTime(t, index_middle, indexTo);
    }


}


