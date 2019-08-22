#include "DriverAVT/AVTStreaming.h"
#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "MatMaker.h"
#include <iostream>


AVTStreaming::AVTStreaming(
        ImageStream& imStream,
        BlockingWait& bw
) : image_stream_{imStream}, blocking_wait_{bw}, ready_{false} { }


void AVTStreaming::init(
        const std::map<std::string, int>& cam_parameters,
        ProcessingTask& task
) {

    VmbErrorType err;
    CameraPtrVector cameras;

    int camera_index = cam_parameters.at("camera_index");
    unsigned long n_frames = (unsigned long)cam_parameters.at("n_frames");

    frames_ = FramePtrVector(n_frames);

    VimbaSystem& sys = VimbaSystem::GetInstance();

    err = sys.GetCameras(cameras);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when getting a list of available cameras");
    }

    unsigned long n_cameras = cameras.size();

    if (n_cameras == 0) {
        throw std::runtime_error("Zero cameras available");
    }

    cam_ = cameras[camera_index];

    std::map<std::string, VmbInt64_t> camera_features;

    err = openCameraWithImageFeatures(cam_, camera_features);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when opening a camera and getting image features");
    }

    MatMaker mm(camera_features);

    unsigned int w, h;
    w = (unsigned int)camera_features["Width"];
    h = (unsigned int)camera_features["Height"];

    image_stream_.init(w, h, mm.getNumberOfChannels());

    IFrameObserverPtr observer(new AVTFrameObserverImageStream(cam_, mm, image_stream_, task));

    err = announceFrames(cam_, frames_, observer);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when announcing frames");
    }

    ready_ = true;

}

void AVTStreaming::operator()() {

    std::cout << "[DEBUG] Starting AVTStreaming thread\n";

    if (!ready_) {
        throw std::runtime_error("AVTStreaming object is not initialized");
    }

    VmbErrorType err;

    err = cam_->StartCapture();
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when starting capture");
    }

    err = queueFrames(cam_, frames_);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when queueing frames");
    }

    err = acquisitionStart(cam_);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when starting acquisition");
    }

    blocking_wait_.wait();

    err = acquisitionStop(cam_);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when stopping acquisition");
    }

    if (streaming_finished_) { // if subscribeToCompletion was called (std::shared_ptr::operator bool)
        streaming_finished_->set_value(true);
    }

    std::cout << "[DEBUG] Stopping AVTStreaming thread\n";

}

std::shared_future<bool> AVTStreaming::subscribeToCompletion() {

    std::shared_ptr<std::promise<bool>> promise_ptr{new std::promise<bool>{}};
    streaming_finished_ = promise_ptr;

    return promise_ptr->get_future();

}

