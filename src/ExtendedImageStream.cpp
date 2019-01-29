#include "ExtendedImageStream.h"

template <class T>
void ExtendedImageStream<T>::init(uint width, uint height, uint numChannels) {

    for (int i = 0; i < this->stream_size; i++) {
        T data{};
        this->processing_data.push_back(data);
    }

    ImageStream::init(width, height, numChannels);

}

template <class T>
void ExtendedImageStream<T>::storeImageData(cv::Mat image, T& data, TimePoint t) {

    std::lock_guard<std::mutex> lock(this->mutex);

    if (!this->ready) {
        throw std::runtime_error("ExtendedImageStream is not yet ready");
    }

    int current_index = this->ctv.getCurrentIndex();
    this->images[current_index] = image;

    this->processing_data[current_index] = data; // copy constructor used

    this->ctv.storeTimestamp(t, 0);
    this->ctv.storeTimestamp(currentTime(), 1);
    this->ctv.advance();

    this->waiting_for_next_image.notify();

}

template <class T>
void ExtendedImageStream<T>::getImage(
        TimePoint t,
        ImageResponse& out,
        T& processing_result
) {

    ImageStream::getImage(t, out);

    processing_result = this->processing_data[out.target_index];

}

template class ExtendedImageStream<int>;
template class ExtendedImageStream<bool>;
template class ExtendedImageStream<double>;
template class ExtendedImageStream<cv::Mat>;
template class ExtendedImageStream<std::vector<cv::Point2f>>;
