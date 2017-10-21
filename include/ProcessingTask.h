#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H

#include <functional>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ImageStream.h"
#include "TimeMeasure.h"
#include "ExtendedImageStream.h"

class ProcessingTask {

public:
    virtual void run(unsigned char* image_ptr, TimePoint t) = 0;

};

class JustStoreTask : public ProcessingTask {

public:

    explicit JustStoreTask(ImageStream& im_stream) : image_stream(im_stream) { }

    void run(unsigned char* image_ptr, TimePoint t) override;

private:
    ImageStream& image_stream;

};

template <class T>
class TypedProcessingTask : public ProcessingTask {

public:

    TypedProcessingTask(
            ExtendedImageStream<T>& im_stream,
            std::function<bool(unsigned char*, ExtendedImageStream<T>&, T&)> f
    );

    void run(unsigned char* image_ptr, TimePoint t) override;

    void copyResult(T& out);

private:

    ExtendedImageStream<T>& image_stream;
    std::function<bool(unsigned char*, ExtendedImageStream<T>&, T&)> function;
    T last_output;

};


#endif
