#ifndef BASICFRAMEOBSERVER_H
#define BASICFRAMEOBSERVER_H

#include <map>
#include <string>
#include "AVTVimba.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <opencv2/opencv.hpp>
#include "MatMaker.h"

class AVTFrameObserverBasic : public IFrameObserver {

public:
    AVTFrameObserverBasic(CameraPtr cam);
    void FrameReceived(FramePtr frame) override;
    virtual void processFrame(FramePtr frame) = 0;

};



#endif
