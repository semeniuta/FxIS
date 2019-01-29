#include "ExtendedImageStream.h"

template <class T>
void ExtendedImageStream<T>::init(uint width, uint height, uint numChannels) {

    for (int i = 0; i < stream_size; i++) {
        T data{};
        processing_data_.push_back(data);
    }

    ImageStream::init(width, height, numChannels);

}

template <class T>
void ExtendedImageStream<T>::storeImageData(cv::Mat image, T& data, TimePoint t) {

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

template <class T>
void ExtendedImageStream<T>::getImage(
        TimePoint t,
        ImageResponse& out,
        T& processing_result
) {

    ImageStream::getImage(t, out);

    processing_result = processing_data_[out.target_index];

}

template class ExtendedImageStream<int>;
template class ExtendedImageStream<bool>;
template class ExtendedImageStream<double>;
template class ExtendedImageStream<cv::Mat>;
template class ExtendedImageStream<std::vector<cv::Point2f>>;
