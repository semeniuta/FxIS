#ifndef VIMBADESCRIBE_H
#define VIMBADESCRIBE_H

#include <string>
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;

VmbErrorType describeVimbaSetup();

VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result);

VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t result);

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector frames, IFrameObserverPtr observer);

VmbErrorType acquisitionStart(CameraPtr cameraPointer);

VmbErrorType acquisitionStop(CameraPtr cameraPointer);

class SimpleFrameObserver : public IFrameObserver {
    SimpleFrameObserver(CameraPtr cameraPointer);
    void FrameReceived(const FramePtr framePointer);
};

#endif
