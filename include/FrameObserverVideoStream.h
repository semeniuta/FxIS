#ifndef FRAMEOBSERVERVIDEOSTREAM_H
#define FRAMEOBSERVERVIDEOSTREAM_H

#include <map>
#include <string>
#include "AVTVimba.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>
#include "MatMaker.h"

class FrameObserverVideoStream : public IFrameObserver {

public:
    FrameObserverVideoStream(CameraPtr cam, MatMaker& mm, std::string windowName);
    void FrameReceived(FramePtr framePointer) override;

private:
    MatMaker& mat_maker;
    std::string window_name;

};

#endif
