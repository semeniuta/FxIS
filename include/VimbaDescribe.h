#ifndef VIMBADESCRIBE_H
#define VIMBADESCRIBE_H

#include <string>
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;

void describeVimbaSetup();
VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result);
VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t result);

#endif
