#include "Service.h"
#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTStreaming.h"
#include <thread>
#include <chrono>

template <class StreamingT, class ResT>
Service<StreamingT, ResT>::Service() : ready(false) { }

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

    auto n_cameras = cam_parameters.size();

    if (task_funcs.size() != n_cameras) {
        throw std::runtime_error("Size mismatch between number of cameras and number of task functions");
    }

    for (int i = 0; i < n_cameras; i++) {

        this->blocking_waits.push_back(
                std::make_unique<BlockingWait>()
        );

        this->image_streams.push_back(
                std::make_unique<ExtendedImageStream<ResT>>(stream_size)
        );

        if (task_funcs.empty()) {

            this->tasks.push_back(
                    std::make_unique<JustStoreTask>( *(this->image_streams[i]) )
            );

        } else {

            this->tasks.push_back(
                    std::make_unique<TypedProcessingTask<ResT>>(
                            *(this->image_streams[i]),
                            task_funcs[i]
                    )
            );

        }

        this->streaming_objects.push_back(
                std::make_unique<StreamingT>(
                        *(this->image_streams[i]),
                        *(this->blocking_waits[i])
                )
        );
        this->streaming_objects[i]->init(
                cam_parameters[i],
                *(tasks[i])
        );
    }

    this->ready = true;

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
void Service<StreamingT, ResT>::grab(std::vector<cv::Mat>& res) {


}

template class Service<AVTStreaming, std::vector<cv::Point2f>>;