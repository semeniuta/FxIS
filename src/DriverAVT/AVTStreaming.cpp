#include "DriverAVT/AVTStreaming.h"
#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "MatMaker.h"
#include <iostream>

void initFramePtrVector(FramePtrVector& v, unsigned long n_frames) {

    v = FramePtrVector(n_frames);

}

AVTStreaming::AVTStreaming(
        ImageStream& imStream,
        BlockingWait& bw
) : image_stream(imStream), blocking_wait(bw), ready(false) { }


void AVTStreaming::init(
        const std::map<std::string, int>& cam_parameters,
        ProcessingTask& task
) {

    VmbErrorType err;
    CameraPtrVector cameras;

    int camera_index = cam_parameters.at("camera_index");
    unsigned long n_frames = (unsigned long)cam_parameters.at("n_frames");

    initFramePtrVector(this->frames, n_frames);

    VimbaSystem& sys = VimbaSystem::GetInstance();

    err = sys.GetCameras(cameras);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when getting a list of available cameras");
    }

    unsigned long n_cameras = cameras.size();

    if (n_cameras == 0) {
        throw std::runtime_error("Zero cameras available");
    }

    this->cam = cameras[camera_index];

    std::map<std::string, VmbInt64_t> camera_features;

    err = openCameraWithImageFeatures(this->cam, camera_features);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when opening a camera and getting image features");
    }

    MatMaker mm(camera_features);

    unsigned int w, h;
    w = (unsigned int)camera_features["Width"];
    h = (unsigned int)camera_features["Height"];

    image_stream.init(w, h, mm.getNumberOfChannels());

    IFrameObserverPtr observer(new AVTFrameObserverImageStream(this->cam, mm, image_stream, task));

    err = announceFrames(this->cam, frames, observer);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when announcing frames");
    }

    this->ready = true;

}

void AVTStreaming::operator()() {

    std::cout << "[DEBUG] Starting AVTStreaming thread\n";

    if (!this->ready) {
        throw std::runtime_error("AVTStreaming object is not initialized");
    }

    VmbErrorType err;

    err = cam->StartCapture();
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when starting capture");
    }

    err = queueFrames(this->cam, this->frames);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when queueing frames");
    }

    err = acquisitionStart(cam);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when starting acquisition");
    }

    this->blocking_wait.wait();

    err = acquisitionStop(cam);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when stopping acquisition");
    }

    if (this->streaming_finished) { // if subscribeToCompletion was called (std::shared_ptr::operator bool)
        this->streaming_finished->set_value(true);
    }

    std::cout << "[DEBUG] Stopping AVTStreaming thread\n";

}

std::shared_future<bool> AVTStreaming::subscribeToCompletion() {

    std::shared_ptr<std::promise<bool>> promise_ptr(new std::promise<bool>{});
    this->streaming_finished = promise_ptr;

    return promise_ptr->get_future();

}

