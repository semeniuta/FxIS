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

template <class ResT>
void typedImageRequestThread(
        ExtendedImageStream<ResT>& im_stream,
        ImageResponse& resp,
        ResT& processing_result,
        ThreadsafeQueue<AsyncImageRequest>& q,
        EventObject& stop_event
) {

    AsyncImageRequest req;

    while (true) {

        q.pop(req);

        im_stream.getImage(
                req.timestamp,
                resp,
                processing_result
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

template <class ResT>
ExtendedImageStreamRequester<ResT>::ExtendedImageStreamRequester(ExtendedImageStream<ResT>& im_stream)
        : image_stream(im_stream) { }


template <class ResT>
void ExtendedImageStreamRequester<ResT>::start() {

    thread_ptr = std::unique_ptr<std::thread>(new std::thread(
            typedImageRequestThread<ResT>,
            std::ref(this->image_stream),
            std::ref(this->image_response),
            std::ref(this->processing_result),
            std::ref(this->q_in),
            std::ref(this->eo_stop)
    ));

    thread_ptr->detach();

}

template <class ResT>
void ExtendedImageStreamRequester<ResT>::copyData(ImageResponse& out_im, ResT& out_proc_res) {

    out_im = this->image_response;
    out_proc_res = this->processing_result;

}

template class ExtendedImageStreamRequester<int>;
template class ExtendedImageStreamRequester<bool>;
template class ExtendedImageStreamRequester<double>;
template class ExtendedImageStreamRequester<cv::Mat>;
template class ExtendedImageStreamRequester<std::vector<cv::Point2f>>;