#include "AVTStreaming.h"
#include "AVTFrameObserverImageStream.h"
#include "MatMaker.h"
#include <iostream>

AVTStreaming::AVTStreaming(int camIndex, unsigned long numFrames, ImageStream& imStream)
        : image_stream(imStream), frames(numFrames) {

    VmbErrorType err;
    CameraPtrVector cameras;

    VimbaSystem& sys = VimbaSystem::GetInstance();

    err = sys.GetCameras(cameras);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when getting a list of available cameras");
    }

    unsigned long n_cameras = cameras.size();

    if (n_cameras == 0) {
        throw std::runtime_error("Zero cameras available");
    }

    this->cam = cameras[camIndex];

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

    IFrameObserverPtr observer(new AVTFrameObserverImageStream(this->cam, mm, image_stream));

    err = announceFrames(this->cam, frames, observer);
    if (err != VmbErrorSuccess) {
        throw std::runtime_error("Exception when announcing frames");
    }

}

