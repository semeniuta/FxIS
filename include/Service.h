
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

    Service() : ready(false), n_cameras(0) { }

    void init(
            unsigned int stream_size,
            const CamerasParameters& cam_parameters,
            std::vector<ProcessingFunction<ResT>> task_funcs = std::vector<ProcessingFunction<ResT>>()
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

    void start() {

        // TODO Look here

        for (auto& streaming_obj_ptr : this->streaming_objects) {

            std::thread t(*streaming_obj_ptr);
            t.detach();
        }

    }

    void stop() {

        for (int i = 0; i < this->n_cameras; i++) {
            this->image_requesters[i]->stop();
        }

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

    void grab(std::vector<ImageResponse>& out_im, std::vector<ResT>& out_proc_res) {

        TimePoint t = currentTime();

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

protected:

    void initBlockingWaits() {

        for (int i = 0; i < this->n_cameras; i++) {

            this->blocking_waits.push_back(
                    std::make_unique<BlockingWait>()
            );

        }

    }

    void initImageStreamsAndTasks(
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

    void initStreamingObjects(const CamerasParameters& cam_parameters) {

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

            std::shared_future<bool> future = this->streaming_objects[i]->subscribeToCompletion();
            this->streaming_finished_futures.push_back(future);

        }

    }

private:

    bool ready;
    unsigned long n_cameras;
    std::vector<std::unique_ptr<ProcessingTask>> tasks;
    std::vector<std::unique_ptr<ExtendedImageStream<ResT>>> image_streams;
    std::vector<std::unique_ptr<ExtendedImageStreamRequester<ResT>>> image_requesters;
    std::vector<std::unique_ptr<StreamingT>> streaming_objects;
    std::vector<std::unique_ptr<BlockingWait>> blocking_waits;
    std::vector<std::shared_future<bool>> streaming_finished_futures;


};


#endif
