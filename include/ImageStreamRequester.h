#ifndef IMAGESTREAMREQUESTER_H
#define IMAGESTREAMREQUESTER_H

#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <future>
#include <opencv2/opencv.hpp>
#include "ImageStream.h"
#include "ThreadsafeQueue.h"
#include "EventObject.h"
#include "TimeMeasure.h"
#include "ExtendedImageStream.h"

struct AsyncImageRequest {
    TimePoint timestamp;
    std::shared_ptr<std::promise<bool>> promise_ptr;
};

template <class ResT>
void extendedImageRequestThread(
        ExtendedImageStream<ResT>& im_stream,
        ImageResponse& resp,
        ResT& processing_result,
        ThreadsafeQueue<AsyncImageRequest>& q,
        EventObject& stop_event
) {

    std::cout << "[DEBUG] Starting extendedImageRequestThread\n";

    AsyncImageRequest req;

    while (true) {

        q.pop(req);

        if (stop_event.hasOccured()) {
            break;
        }

        im_stream.getImage(
                req.timestamp,
                resp,
                processing_result
        );

        req.promise_ptr->set_value(true);

    }

    std::cout << "[DEBUG] Stopping extendedImageRequestThread\n";

}

template <class ResT>
class ExtendedImageStreamRequester {

public:

    explicit ExtendedImageStreamRequester(ExtendedImageStream<ResT>& im_stream) : image_stream_(im_stream) { }

    void start() {

        thread_ptr_ = std::make_unique<std::thread>(
                extendedImageRequestThread<ResT>,
                std::ref(image_stream_),
                std::ref(image_response_),
                std::ref(processing_result_),
                std::ref(q_in_),
                std::ref(eo_stop_)
        );

        thread_ptr_->detach();

    }

    void stop() {

        eo_stop_.notify();

        // TODO Make the stopping fix more elegant

        std::shared_ptr<std::promise<bool>> promise_ptr(new std::promise<bool>{});
        AsyncImageRequest req{
                currentTime(),
                promise_ptr
        };

        q_in_.push(req);

    }

    std::future<bool> requestImage(TimePoint t) {

        std::shared_ptr<std::promise<bool>> promise_ptr(new std::promise<bool>{});
        AsyncImageRequest req{
                t,
                promise_ptr
        };

        std::future<bool> future = req.promise_ptr->get_future();

        q_in_.push(req);

        return future;

    };

    void copyData(ImageResponse& out_im, ResT& out_proc_res) {

        out_im = image_response_;
        out_proc_res = processing_result_;

    }

    void copyData(ImageResponse& out_im) {

        out_im = image_response_;

    }

private:

    ThreadsafeQueue<AsyncImageRequest> q_in_;
    EventObject eo_stop_;
    std::unique_ptr<std::thread> thread_ptr_;

    ExtendedImageStream<ResT>& image_stream_;
    ImageResponse image_response_;
    ResT processing_result_;

};


#endif
