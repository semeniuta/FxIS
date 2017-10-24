#include "ProcessingTask.h"

void JustStoreTask::run(cv::Mat image, TimePoint t) {

    this->image_stream.storeImageData(image, t);

}

template <class T>
TypedProcessingTask<T>::TypedProcessingTask(
        ExtendedImageStream<T>& im_stream,
        std::function<bool(cv::Mat, ExtendedImageStream<T>&, T&)> f
) : image_stream(im_stream), function(f) { };

template <class T>
void TypedProcessingTask<T>::run(cv::Mat image, TimePoint t) {

    bool do_save = this->function(image, this->image_stream, this->last_output);

    if (do_save) {
        this->image_stream.storeImageData(image, this->last_output, t);
    }

}

template <class T>
void TypedProcessingTask<T>::copyResult(T& out) {

    out = this->last_output;

}

template class TypedProcessingTask<std::vector<cv::Point2f>>;