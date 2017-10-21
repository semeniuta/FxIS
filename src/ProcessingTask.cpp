#include "ProcessingTask.h"


void JustStoreTask::run(unsigned char* image_ptr, TimePoint t) {

    this->image_stream.storeImageData(image_ptr, t);

}

template <class T>
void TypedProcessingTask<T>::run(unsigned char* image_ptr, TimePoint t) {

    bool do_save = this->function(image_ptr, this->image_stream, this->last_output);

    if (do_save) {
        this->image_stream.storeImageData(image_ptr, t);
    }

}

template <class T>
void TypedProcessingTask<T>::copyResult(T& out) {

    out = this->last_output;

}