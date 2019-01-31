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
    virtual void run(cv::Mat image, TimePoint t) = 0;

};

class JustStoreTask : public ProcessingTask {

public:

    explicit JustStoreTask(ImageStream& im_stream) : image_stream_(im_stream) { }

    void run(cv::Mat image, TimePoint t) override {

        image_stream_.storeImageData(image, t);

    }

private:
    ImageStream& image_stream_;

};

template <class T>
class TypedProcessingTask : public ProcessingTask {

public:

    TypedProcessingTask(
            ExtendedImageStream<T>& im_stream,
            std::function<bool(cv::Mat, ExtendedImageStream<T>&, T&)> f
    ) : image_stream_(im_stream), function_(f) { }

    void run(cv::Mat image, TimePoint t) override {

        bool do_save = function_(image, image_stream_, last_output_);

        if (do_save) {
            image_stream_.storeImageData(image, last_output_, t);
        }

    };

    void copyResult(T& out) {

        out = last_output_;

    }

private:

    ExtendedImageStream<T>& image_stream_;
    std::function<bool(cv::Mat, ExtendedImageStream<T>&, T&)> function_;
    T last_output_;

};


#endif
