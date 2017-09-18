#ifndef AVTSTREAMING_H
#define AVTSTREAMING_H

#include <vector>
//#include <mutex>
#include "AVTVimba.h"
#include "MatMaker.h"
#include "ImageStream.h"

class AVTStreaming {

public:
    AVTStreaming(int camIndex, unsigned long numFrames, ImageStream& imStream);
    void operator()() {

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

        std::cout<< "Press <enter> to stop acquisition...\n" ;
        getchar();

        err = acquisitionStop(cam);
        if (err != VmbErrorSuccess) {
            throw std::runtime_error("Exception when stopping acquisition");
        }

    }

private:
    ImageStream& image_stream;
    CameraPtr cam;
    FramePtrVector frames;
    //std::condition_variable trigger_stop;

};


#endif

