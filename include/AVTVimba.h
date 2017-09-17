#ifndef VIMBADESCRIBE_H
#define VIMBADESCRIBE_H

#include <string>
#include <map>
#include <vector>
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;

VmbErrorType startupVimbaSystemAndGetCameras(VimbaSystem& sys, CameraPtrVector& cameras);

VmbErrorType describeVimbaSetup();

VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result);

VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t& result);

VmbErrorType announceFrames(CameraPtr cameraPointer, FramePtrVector& frames, IFrameObserverPtr observer);

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector& frames);

VmbErrorType openCamera(CameraPtr cameraPointer, VmbAccessModeType accessMode);

VmbErrorType openCameraWithImageFeatures(CameraPtr cam, std::map<std::string, VmbInt64_t>& featuresMap);

VmbErrorType acquisitionStart(CameraPtr cameraPointer);

VmbErrorType acquisitionStop(CameraPtr cameraPointer);

VmbErrorType streamingStart(CameraPtr cam, FramePtrVector frames, IFrameObserverPtr observer);

VmbErrorType getFeaturesMap(CameraPtr cam, const std::vector<std::string>& names, std::map<std::string, VmbInt64_t>& out_map);

#endif
