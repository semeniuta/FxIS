#ifndef FRAMEOBSERVERVIDEOSTREAM_H
#define FRAMEOBSERVERVIDEOSTREAM_H

#include <map>
#include <string>
#include "AVTVimba.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>
#include "MatMaker.h"
#include "AVTFrameObserverBasic.h"

class AVTFrameObserverVideoStream : public AVTFrameObserverBasic {

public:
    AVTFrameObserverVideoStream(CameraPtr cam, MatMaker mm, std::string windowName);
    void processFrame(FramePtr frame) override;

private:
    MatMaker mat_maker;
    std::string window_name;

};

#endif
