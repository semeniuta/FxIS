#include "ProcessingTask.h"

void JustStoreTask::run(unsigned char* image_ptr, TimePoint t) {

    this->image_stream.storeImageData(image_ptr, t);

}

template <class T>
TypedProcessingTask<T>::TypedProcessingTask(
        ExtendedImageStream<T>& im_stream,
        std::function<bool(unsigned char*, ExtendedImageStream<T>&, T&)> f
) : image_stream(im_stream), function(f) { };

template <class T>
void TypedProcessingTask<T>::run(unsigned char* image_ptr, TimePoint t) {

    bool do_save = this->function(image_ptr, this->image_stream, this->last_output);

    if (do_save) {
        this->image_stream.storeImageData(image_ptr, this->last_output, t);
    }

}

template <class T>
void TypedProcessingTask<T>::copyResult(T& out) {

    out = this->last_output;

}

template class TypedProcessingTask<std::vector<cv::Point2f>>;