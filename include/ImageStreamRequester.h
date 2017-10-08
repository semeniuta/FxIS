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

struct ImageResponse {
    cv::Mat image;
    TimestampsMatrix timestamps_snapshot;
    unsigned long target_index;
    unsigned long current_index;
    std::vector<TimePoint> time_measurements;
};

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

    ImageStreamRequester(ImageStream& im_stream);

    std::future<bool> requestImage(TimePoint t);

    void copyData(
            cv::Mat& im,
            TimestampsMatrix& timestampsCopy,
            unsigned long& index,
            unsigned long& current_index,
            std::vector<TimePoint>& timeMeasurements
    );

private:

    ImageStream& image_stream;
    ThreadsafeQueue<AsyncImageRequest> q_in;
    EventObject eo_stop;
    std::unique_ptr<std::thread> thread_ptr;

    ImageResponse image_response;

};


#endif
