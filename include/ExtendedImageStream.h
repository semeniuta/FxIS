#ifndef EXTENDEDIMAGESTREAM_H
#define EXTENDEDIMAGESTREAM_H

#include "ImageStream.h"

template <class T>
class ExtendedImageStream : public ImageStream {

public:

    explicit ExtendedImageStream(unsigned int size) : ImageStream(size) {}

    void init(uint width, uint height, uint numChannels) override {

        for (int i = 0; i < stream_size; i++) {
            T data{};
            processing_data_.push_back(data);
        }

        ImageStream::init(width, height, numChannels);

    }

    void storeImageData(cv::Mat image, T& data, TimePoint t) {

        std::lock_guard<std::mutex> lock(mutex_);

        if (!ready_) {
            throw std::runtime_error("ExtendedImageStream is not yet ready");
        }

        int current_index = ctv_.getCurrentIndex();
        images_[current_index] = image;

        processing_data_[current_index] = data; // copy constructor used

        ctv_.storeTimestamp(t, 0);
        ctv_.storeTimestamp(currentTime(), 1);
        ctv_.advance();

        waiting_for_next_image_.notify();

    }

    void getImage(
            TimePoint t,
            ImageResponse& out,
            T& processing_result
    ) {

        ImageStream::getImage(t, out);

        processing_result = processing_data_[out.target_index];

    }

private:

    std::vector<T> processing_data_;

};


#endif
