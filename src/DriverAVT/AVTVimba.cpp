#include <iostream>
#include <string>
#include <sstream>

#include "DriverAVT/AVTVimba.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

using namespace AVT::VmbAPI;

VmbErrorType getFeatureValue(CameraPtr cameraPointer, std::string featureName, VmbInt64_t& result) {

    FeaturePtr feature_pointer;
    VmbErrorType err;
    VmbInt64_t tmp_result;

    err = cameraPointer->GetFeatureByName(featureName.c_str(), feature_pointer);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->GetFeatureByName" << std::endl;
        return err;
    }

    err = feature_pointer->GetValue(tmp_result);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] feature_pointer->GetValue" << std::endl;
        return err;
    }
    result = tmp_result;

    return VmbErrorSuccess;
}

VmbErrorType getFeaturesMap(CameraPtr cam, const std::vector<std::string>& names, std::map<std::string, VmbInt64_t>& out_map) {

    VmbErrorType err;

    VmbInt64_t val;
    for (const std::string& name : names) {

        err = getFeatureValue(cam, name, val);
        if (err != VmbErrorSuccess) {
            return err;
        }
        out_map[name] = val;
    }

    return VmbErrorSuccess;

}

VmbErrorType announceFrames(CameraPtr cameraPointer, FramePtrVector& frames, IFrameObserverPtr observer) {

    VmbErrorType err;
    VmbInt64_t payload_size;

    err = getFeatureValue(cameraPointer, "PayloadSize", payload_size);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] getFeatureValue" << std::endl;
        return err;
    }

    for (FramePtr& frame : frames) {
        frame.reset(new Frame(payload_size));

        err = frame->RegisterObserver(observer);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] frame->RegisterObserver" << std::endl;
            return err;
        }

        err = cameraPointer->AnnounceFrame(frame);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] cameraPointer->AnnounceFrame" << std::endl;
            return err;
        }
    }

    return VmbErrorSuccess;

}

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector& frames) {

    VmbErrorType err;

    for (FramePtr& frame : frames) {
        err = cameraPointer->QueueFrame(frame);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] cameraPointer->QueueFrame" << std::endl;
            return err;
        }
    }

    return VmbErrorSuccess;
}

VmbErrorType acquisitionStart(CameraPtr cameraPointer) {

    VmbErrorType err;
    FeaturePtr start_feature;

    err = cameraPointer->GetFeatureByName("AcquisitionStart", start_feature);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->GetFeatureByName" << std::endl;
        return err;
    }

    err = start_feature->RunCommand();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] start_feature->RunCommand()" << std::endl;
        return err;
    }

    return VmbErrorSuccess;


}

VmbErrorType acquisitionStop(CameraPtr cameraPointer) {

    VmbErrorType err;
    FeaturePtr stop_feature;

    err = cameraPointer->GetFeatureByName("AcquisitionStop", stop_feature);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->GetFeatureByName" << std::endl;
        return err;
    }

    err = stop_feature->RunCommand();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] stop_feature->RunCommand()" << std::endl;
        return err;
    }

    err = cameraPointer->EndCapture();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] stop_feature->RunCommand()" << std::endl;
        return err;
    }

    err = cameraPointer->FlushQueue();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->FlushQueue" << std::endl;
        return err;
    }

    err = cameraPointer->RevokeAllFrames();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->RevokeAllFrames" << std::endl;
        return err;
    }

    err = cameraPointer->Close();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->Close" << std::endl;
        return err;
    }

    return VmbErrorSuccess;

}


VmbErrorType streamingStart(CameraPtr cam, FramePtrVector frames, IFrameObserverPtr observer) {

    VmbErrorType err;

    err = announceFrames(cam, frames, observer);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] announceFrames(cam, frames, observer)" << std::endl;
        return err;
    }

    err = cam->StartCapture();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cam->StartCapture()" << std::endl;
        return err;
    }

    err = queueFrames(cam, frames);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] queueFrames(cam, frames)" << std::endl;
        return err;
    }

    err = acquisitionStart(cam);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] acquisitionStart(cam)" << std::endl;
        return err;
    }

    return VmbErrorSuccess;

}


VmbErrorType describeVimbaCamera(CameraPtr cameraPointer, std::string& result) {

    std::stringstream buffer;
    std::string param;
    VmbErrorType err;

    err = cameraPointer->GetName(param);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->GetName" << std::endl;
        return err;
    }
    buffer << param << " ";

    err = cameraPointer->GetID(param);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->GetID" << std::endl;
        return err;
    }
    buffer << param << " ";

    buffer << std::endl;

    result =  buffer.str();
    return VmbErrorSuccess;
}


VmbErrorType startupVimbaSystemAndGetCameras(VimbaSystem& sys, CameraPtrVector& cameras) {

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] sys.Startup" << std::endl;
        return err;
    }

    err = sys.GetCameras(cameras);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] sys.GetCameras" << std::endl;
        return err;
    }

    return VmbErrorSuccess;

}


VmbErrorType describeVimbaSetup() {

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;
    CameraPtrVector cameras;
    std::string cam_description;

    err = startupVimbaSystemAndGetCameras(sys, cameras);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] startupVimbaSystemAndGetCameras" << std::endl;
        sys.Shutdown();
        return err;
    }

    std::cout << cameras.size() << " cameras available:" << std::endl;

    for (CameraPtr& cam : cameras) {

        err = describeVimbaCamera(cam, cam_description);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] describeVimbaCamera" << std::endl;
            sys.Shutdown();
            return err;
        }
        std::cout << cam_description;
    }

    sys.Shutdown();
    return VmbErrorSuccess;

}

VmbErrorType openCamera(CameraPtr cameraPointer, VmbAccessModeType accessMode) {
    VmbErrorType err;

    err = cameraPointer->Open(accessMode);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] cameraPointer->Open" << std::endl;
        return err;
    }

    return VmbErrorSuccess;
}

VmbErrorType openCameraWithImageFeatures(CameraPtr cam, std::map<std::string, VmbInt64_t>& featuresMap) {

    VmbErrorType err;

    err = openCamera(cam, VmbAccessModeFull);
    if (err != VmbErrorSuccess) {
        return err;
    }

    std::vector<std::string> feature_names = {"Height", "Width", "PixelFormat"};

    err = getFeaturesMap(cam, feature_names, featuresMap);
    if (err != VmbErrorSuccess) {
        return err;
    }

    return VmbErrorSuccess;


}

VmbErrorType getCamerasByIndices(const std::vector<unsigned int>& indices, CameraPtrVector& cameras) {

    VmbErrorType err;

    CameraPtrVector all_cameras;

    VimbaSystem& sys = VimbaSystem::GetInstance();

    err = sys.GetCameras(all_cameras);
    if (err != VmbErrorSuccess) {
        return err;
    }

    unsigned long n_cameras = all_cameras.size();

    if (n_cameras == 0) {
        return VmbErrorOther;
    }

    for (auto& idx : indices) {
        if (idx < n_cameras)
            cameras.push_back( all_cameras[idx] );
    }

    return VmbErrorSuccess;

}

VmbErrorType getCamerasByIP(const std::vector<std::string>& ip_addresses, CameraPtrVector& cameras) {

    VmbErrorType err;

    VimbaSystem& sys = VimbaSystem::GetInstance();

    for (const auto& ip : ip_addresses) {
        CameraPtr cam;

        err = sys.GetCameraByID(ip.c_str(), cam);
        if (err != VmbErrorSuccess)
            return err;

        cameras.push_back(cam);
    }

    return VmbErrorSuccess;

}