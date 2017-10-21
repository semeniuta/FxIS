#ifndef PROCESSINGTASK_H
#define PROCESSINGTASK_H

#include <opencv2/opencv.hpp>
#include "ImageStream.h"
#include "TimeMeasure.h"

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
            ImageStream& im_stream,
            bool (*f)(unsigned char* p_im, ImageStream& is, T& out)
    ) : image_stream(im_stream), function(f) { };

    void run(unsigned char* image_ptr, TimePoint t) override;

    void copyResult(T& out);

private:

    ImageStream& image_stream;
    bool (*function)(unsigned char* p_im, ImageStream& is, T& out);
    T last_output;

};


#endif
