#ifndef VIMBADESCRIBE_H
#define VIMBADESCRIBE_H

#include <string>
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;

VmbErrorType startupVimbaSystemAndGetCameras(VimbaSystem& sys, CameraPtrVector& cameras);

VmbErrorType describeVimbaSetup();

VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result);

VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t& result);

VmbErrorType announceFrames(CameraPtr cameraPointer, FramePtrVector& frames, IFrameObserverPtr observer);

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector& frames);

VmbErrorType openCamera(CameraPtr cameraPointer, VmbAccessModeType accessMode);

VmbErrorType acquisitionStart(CameraPtr cameraPointer);

VmbErrorType acquisitionStop(CameraPtr cameraPointer);

class SimpleFrameObserver : public IFrameObserver {

public:
    SimpleFrameObserver(CameraPtr cameraPointer);
    void FrameReceived(const FramePtr framePointer);

};

#endif
