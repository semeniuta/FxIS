#include "ImageStreamRequester.h"

void imageRequestThread(
        ImageStream& im_stream,
        ImageResponse& resp,
        ThreadsafeQueue<AsyncImageRequest>& q,
        EventObject& stop_event
) {

    AsyncImageRequest req;

    while (true) {

        q.pop(req);

        im_stream.getImage(
                req.timestamp,
                resp
        );

        req.promise_ptr->set_value(true);

        if (stop_event.hasOccured()) {
            break;
        }
    }

}

ImageStreamRequester::ImageStreamRequester(ImageStream& im_stream)
        : image_stream(im_stream) { }

void ImageStreamRequester::start() {

    thread_ptr = std::unique_ptr<std::thread>(new std::thread(
            imageRequestThread,
            std::ref(this->image_stream),
            std::ref(this->image_response),
            std::ref(this->q_in),
            std::ref(this->eo_stop)
    ));

    thread_ptr->detach();

}

void ImageStreamRequester::copyData(ImageResponse& out) {

    out = this->image_response;

}