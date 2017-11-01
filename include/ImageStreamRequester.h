#ifndef IMAGESTREAMREQUESTER_H
#define IMAGESTREAMREQUESTER_H

#include <vector>
#include <thread>
#include <memory>
#include <future>
#include <opencv2/opencv.hpp>
#include "ImageStream.h"
#include "ThreadsafeQueue.h"
#include "EventObject.h"
#include "TimeMeasure.h"

struct AsyncImageRequest {
    TimePoint timestamp;
    std::shared_ptr<std::promise<bool>> promise_ptr;
};

void imageRequestThread(
        ImageStream& im_stream,
        ImageResponse& resp,
        ThreadsafeQueue<AsyncImageRequest>& q,
        EventObject& stop_event
);

class ImageStreamRequester {

public:

    explicit ImageStreamRequester(ImageStream& im_stream);

    std::future<bool> requestImage(TimePoint t);

    void copyData(ImageResponse& out);

private:

    ImageStream& image_stream;
    ThreadsafeQueue<AsyncImageRequest> q_in;
    EventObject eo_stop;
    std::unique_ptr<std::thread> thread_ptr;

    ImageResponse image_response;

};


#endif
