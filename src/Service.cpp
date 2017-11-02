#include "Service.h"
#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTStreaming.h"
#include <thread>
#include <chrono>

template <class StreamingT, class ResT>
Service<StreamingT, ResT>::Service() : ready(false), n_cameras(0) { }

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::init(
        unsigned int stream_size,
        const CamerasParameters& cam_parameters,
        std::vector<ProcessingFunction<ResT>> task_funcs
) {

    VimbaSystem& sys = VimbaSystem::GetInstance();

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        sys.Shutdown();
        throw std::runtime_error("Failed to start up Vimba system");
    }

    this->n_cameras = cam_parameters.size();

    if (task_funcs.size() != n_cameras) {
        throw std::runtime_error("Size mismatch between number of cameras and number of task functions");
    }

    initBlockingWaits();
    initImageStreamsAndTasks(stream_size, task_funcs);
    initStreamingObjects(cam_parameters);

    this->ready = true;

}


template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::initBlockingWaits() {

    for (int i = 0; i < this->n_cameras; i++) {

        this->blocking_waits.push_back(
                std::make_unique<BlockingWait>()
        );

    }

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::initImageStreamsAndTasks(
        unsigned int stream_size,
        const std::vector<ProcessingFunction<ResT>>& task_funcs
) {

    for (int i = 0; i < this->n_cameras; i++) {

        this->image_streams.push_back(
                std::make_unique<ExtendedImageStream<ResT>>(stream_size)
        );

        this->image_requesters.push_back(
                std::make_unique<ExtendedImageStreamRequester<ResT>>(
                        *(this->image_streams[i])
                )
        );
        this->image_requesters[i]->start();

        this->tasks.push_back(
                std::make_unique<TypedProcessingTask<ResT>>(
                        *(this->image_streams[i]),
                        task_funcs[i]
                )
        );

    }

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::initStreamingObjects(const CamerasParameters& cam_parameters) {

    for (int i = 0; i < this->n_cameras; i++) {

        this->streaming_objects.push_back(
                std::make_unique<StreamingT>(
                        *(this->image_streams[i]),
                        *(this->blocking_waits[i])
                )
        );
        this->streaming_objects[i]->init(
                cam_parameters[i],
                *(this->tasks[i])
        );

    }

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::start() {

    for (auto& streaming_obj_ptr : this->streaming_objects) {

        std::shared_future<bool> future = streaming_obj_ptr->subscribeToCompletion();
        this->streaming_finished_futures.push_back(future);

        std::thread t(*streaming_obj_ptr);
        t.detach();
    }

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::stop() {

    for (auto& bw : this->blocking_waits) {
        bw->notify();
    }

    std::stack<std::shared_future<bool>> not_ready_futures;
    for (std::shared_future<bool> f : this->streaming_finished_futures) {
        not_ready_futures.push(f);
    }

    while(!not_ready_futures.empty()) {
        not_ready_futures.top().wait();;
        not_ready_futures.pop();
    }

    VimbaSystem& sys = VimbaSystem::GetInstance();
    sys.Shutdown();
    std::cout << "Shutting down normally\n";

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::grab(std::vector<ImageResponse>& out_im, std::vector<ResT>& out_proc_res) {

    TimePoint t;

    std::vector<std::future<bool>> all_futures;

    for (auto& ir : this->image_requesters) {
        all_futures.push_back(ir->requestImage(t));
    }

    ImageResponse im_resp;
    ResT proc_res;

    for (int i = 0; i < this->n_cameras; i++) {

        all_futures[i].wait();

        this->image_requesters[i]->copyData(im_resp, proc_res);

        out_im.push_back(im_resp);
        out_proc_res.push_back(proc_res);
    }

}

template class Service<AVTStreaming, int>;
template class Service<AVTStreaming, bool>;
template class Service<AVTStreaming, double>;
template class Service<AVTStreaming, cv::Mat>;
template class Service<AVTStreaming, std::vector<cv::Point2f>>;
