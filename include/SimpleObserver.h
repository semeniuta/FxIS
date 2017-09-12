#ifndef SIMPLEOBSERVER_H

#include "AVTVimba.h"
#include "VimbaCPP/Include/VimbaCPP.h"

class SimpleFrameObserver : public IFrameObserver {

public:
    SimpleFrameObserver(CameraPtr cameraPointer);
    void FrameReceived(FramePtr framePointer) override;

};

#define SIMPLEOBSERVER_H

#endif
