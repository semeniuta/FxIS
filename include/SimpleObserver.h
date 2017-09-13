#ifndef SIMPLEOBSERVER_H
#define SIMPLEOBSERVER_H

#include <map>
#include <string>
#include "AVTVimba.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>
#include "MatMaker.h"

class SimpleFrameObserver : public IFrameObserver {

public:
    SimpleFrameObserver(CameraPtr cameraPointer, MatMaker& mm);
    void FrameReceived(FramePtr framePointer) override;

private:
    MatMaker& mat_maker;

};


#endif
