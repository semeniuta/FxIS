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

void imageRequestThread(
        ImageStream& im_stream,
        ImageResponse& resp,
        ThreadsafeQueue<AsyncImageRequest>& q,
        EventObject& stop_event
);

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

    explicit ExtendedImageStreamRequester(ExtendedImageStream<ResT>& im_stream) : image_stream(im_stream) { }

    void start() {

        thread_ptr = std::unique_ptr<std::thread>(new std::thread(
                extendedImageRequestThread<ResT>,
                std::ref(this->image_stream),
                std::ref(this->image_response),
                std::ref(this->processing_result),
                std::ref(this->q_in),
                std::ref(this->eo_stop)
        ));

        thread_ptr->detach();

    }

    void stop() {

        this->eo_stop.notify();

        // TODO Make the stopping fix more elegant

        std::shared_ptr<std::promise<bool>> promise_ptr(new std::promise<bool>{});
        AsyncImageRequest req{
                currentTime(),
                promise_ptr
        };

        this->q_in.push(req);

    }

    std::future<bool> requestImage(TimePoint t) {

        std::shared_ptr<std::promise<bool>> promise_ptr(new std::promise<bool>{});
        AsyncImageRequest req{
                t,
                promise_ptr
        };

        std::future<bool> future = req.promise_ptr->get_future();

        this->q_in.push(req);

        return future;

    };

    void copyData(ImageResponse& out_im, ResT& out_proc_res) {

        out_im = this->image_response;
        out_proc_res = this->processing_result;

    }

    void copyData(ImageResponse& out_im) {

        out_im = this->image_response;

    }

private:

    ThreadsafeQueue<AsyncImageRequest> q_in;
    EventObject eo_stop;
    std::unique_ptr<std::thread> thread_ptr;

    ExtendedImageStream<ResT>& image_stream;
    ImageResponse image_response;
    ResT processing_result;

};


#endif
