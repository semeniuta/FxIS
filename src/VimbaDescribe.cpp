#include <iostream>
#include <string>
#include <sstream>

#include "VimbaDescribe.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

using namespace AVT::VmbAPI;

VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t result) {

    FeaturePtr feature_pointer;
    VmbErrorType err;
    VmbInt64_t tmp_result;

    err = cameraPointer->GetFeatureByName(featureName.c_str(), feature_pointer);
    if (err == VmbErrorSuccess) {
        err = feature_pointer->GetValue(tmp_result);
        if (err == VmbErrorSuccess) {
            result = tmp_result;
        }
        else {
            return err;
        }
    } else {
        return err;
    }

    return VmbErrorSuccess;
}

VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result) {

    std::stringstream buffer;
    std::string param;
    VmbErrorType err;

    err = cameraPointer->GetName(param);
    if (err == VmbErrorSuccess) {
        buffer << param << " ";
    } else {
        return err;
    }

    err = cameraPointer->GetID(param);
    if (err == VmbErrorSuccess) {
        buffer << param << " ";
    } else {
        return err;
    }

    buffer << std::endl;
    result =  buffer.str();
    return VmbErrorSuccess;
}

void describeVimbaSetup() {

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;
    CameraPtrVector cameras;
    std::string cam_description;

    err = sys.Startup();
    if (err == VmbErrorSuccess) {

        sys.GetCameras(cameras);
        if (err ==  VmbErrorSuccess) {

            std::cout << cameras.size() << " cameras available:" << std::endl;

            for (CameraPtrVector::iterator itr = cameras.begin(); itr != cameras.end(); itr++) {
                CameraPtr cam_pointer = *itr;

                err = describeVimbaCamera(cam_pointer, cam_description);
                if (err == VmbErrorSuccess) {
                    std::cout << cam_description;
                }
            }

        }
    }

    sys.Shutdown();

}
