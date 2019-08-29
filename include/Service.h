
#ifndef SERVICE_H
#define SERVICE_H

#include "ImageStream.h"
#include "ImageStreamRequester.h"
#include "ProcessingTask.h"
#include "BlockingWait.h"
#include "DriverAVT/AVTVimba.h"
#include <vector>
#include <stack>
#include <memory>
#include <future>
#include <functional>
#include <thread>
#include <opencv2/opencv.hpp>

using CamerasParameters = std::vector<std::map<std::string, int>>;

template <class ResT>
using ProcessingFunction = std::function<bool(cv::Mat, ExtendedImageStream<ResT>&, ResT&)>;

template <class StreamingT, class ResT>
class Service {

public:

    Service() : ready_(false), n_cameras_(0) { }

    void init(
            unsigned int stream_size,
            const CamerasParameters& cam_parameters,
            std::vector<ProcessingFunction<ResT>> task_funcs = std::vector<ProcessingFunction<ResT>>()
    ) {

        // It used to be Vimba statup here

        n_cameras_ = cam_parameters.size();

        if (task_funcs.size() != n_cameras_) {
            throw std::runtime_error("Size mismatch between number of cameras and number of task functions");
        }

        initBlockingWaits();
        initImageStreamsAndTasks(stream_size, task_funcs);
        initStreamingObjects(cam_parameters);

        ready_ = true;

    }

    void start() {

        // TODO Look here

        for (auto& streaming_obj_ptr : streaming_objects_) {

            std::thread t(*streaming_obj_ptr);
            t.detach();
        }

    }

    void stop() {

        for (int i = 0; i < n_cameras_; i++) {
            image_requesters_[i]->stop();
        }

        for (auto& bw : blocking_waits_) {
            bw->notify();
        }

        std::stack<std::shared_future<bool>> not_ready_futures;
        for (const auto& f : streaming_finished_futures_) {
            not_ready_futures.push(f);
        }

        while(!not_ready_futures.empty()) {
            not_ready_futures.top().wait();
            not_ready_futures.pop();
        }

        // It used to be Vimba shut down here

    }

    void grab(std::vector<ImageResponse>& out_im, std::vector<ResT>& out_proc_res) {

        TimePoint t = currentTime();

        std::vector<std::future<bool>> all_futures;

        for (auto& ir : image_requesters_) {
            all_futures.push_back(ir->requestImage(t));
        }

        ImageResponse im_resp;
        ResT proc_res;

        for (int i = 0; i < n_cameras_; i++) {

            all_futures[i].wait();

            image_requesters_[i]->copyData(im_resp, proc_res);

            out_im.push_back(im_resp);
            out_proc_res.push_back(proc_res);
        }

    }

protected:

    void initBlockingWaits() {

        for (int i = 0; i < n_cameras_; i++) {

            blocking_waits_.push_back(
                    std::make_unique<BlockingWait>()
            );

        }

    }

    void initImageStreamsAndTasks(
            unsigned int stream_size,
            const std::vector<ProcessingFunction<ResT>>& task_funcs
    ) {

        for (int i = 0; i < n_cameras_; i++) {

            image_streams_.push_back(
                    std::make_unique<ExtendedImageStream<ResT>>(stream_size)
            );

            image_requesters_.push_back(
                    std::make_unique<ExtendedImageStreamRequester<ResT>>(
                            *(image_streams_[i])
                    )
            );
            image_requesters_[i]->start();

            tasks_.push_back(
                    std::make_unique<TypedProcessingTask<ResT>>(
                            *(image_streams_[i]),
                            task_funcs[i]
                    )
            );

        }

    }

    void initStreamingObjects(const CamerasParameters& cam_parameters) {

        for (int i = 0; i < n_cameras_; i++) {

            streaming_objects_.push_back(
                    std::make_unique<StreamingT>(
                            *(image_streams_[i]),
                            *(blocking_waits_[i])
                    )
            );

            streaming_objects_[i]->init(
                    cam_parameters[i],
                    *(tasks_[i])
            );

            std::shared_future<bool> future = streaming_objects_[i]->subscribeToCompletion();
            streaming_finished_futures_.push_back(future);

        }

    }

private:

    bool ready_;
    unsigned long n_cameras_;
    std::vector<std::unique_ptr<ProcessingTask>> tasks_;
    std::vector<std::unique_ptr<ExtendedImageStream<ResT>>> image_streams_;
    std::vector<std::unique_ptr<ExtendedImageStreamRequester<ResT>>> image_requesters_;
    std::vector<std::unique_ptr<StreamingT>> streaming_objects_;
    std::vector<std::unique_ptr<BlockingWait>> blocking_waits_;
    std::vector<std::shared_future<bool>> streaming_finished_futures_;


};


#endif
