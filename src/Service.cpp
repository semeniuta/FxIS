#include "Service.h"
#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTStreaming.h"
#include <thread>

template <class StreamingT, class ResT>
Service<StreamingT, ResT>::Service() : ready(false) { }

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::init(
        unsigned int stream_size,
        const CamerasParameters& cam_parameters,
        std::vector<ProcessingTask>& tasks
) {

    VimbaSystem& sys = VimbaSystem::GetInstance();

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        sys.Shutdown();
        throw std::runtime_error("Failed to start up Vimba system");
    }

    auto n_cameras = cam_parameters.size();



    for (int i = 0; i < n_cameras; i++) {

        this->blocking_waits.push_back(
                std::make_unique<BlockingWait>()
        );

        this->image_streams.push_back(
                std::make_unique<ExtendedImageStream<ResT>>(stream_size)
        );

        this->streaming_objects.push_back(
                std::make_unique<StreamingT>(
                        *(this->image_streams[i]),
                        *(this->blocking_waits[i])
                )
        );
        this->streaming_objects[i]->init(cam_parameters[i], tasks[i]);
    }

    this->ready = true;

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::start() {

    for (auto& streaming_obj_ptr : this->streaming_objects) {

        //std::future<bool> future = streaming_obj_ptr->subscribeToCompletion();
        //this->streaming_funished_futures.push_back(future);

        std::thread t(*streaming_obj_ptr);
        t.detach();
    }

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::stop() {

    for (auto& bw : this->blocking_waits) {
        bw->notify();
    }

    // TODO Check futures

}

template <class StreamingT, class ResT>
void Service<StreamingT, ResT>::grab(std::vector<cv::Mat>& res) {


}

template class Service<AVTStreaming, std::vector<cv::Point2f>>;