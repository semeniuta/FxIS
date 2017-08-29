#include <iostream>
#include <string>
#include <sstream>

#include "AVTVimba.h"

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

VmbErrorType announceFrames(CameraPtr cameraPointer, FramePtrVector& frames, IFrameObserverPtr& observer) {

    VmbErrorType err;
    VmbInt64_t payload_size;

    err = getFeatureValue(cameraPointer, "PayloadSize", payload_size);
    if (err != VmbErrorSuccess) {
        std::cout << "[ERROR] getFeatureValue" << std::endl;
        return err;
    }

    for (FramePtrVector::iterator itr = frames.begin(); itr != frames.end(); itr++) {

        (*itr).reset(new Frame(payload_size));

        err = (*itr)->RegisterObserver(observer);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] frame->RegisterObserver" << std::endl;
            return err;
        }

        err = cameraPointer->AnnounceFrame(*itr);
        if (err != VmbErrorSuccess) {
            std::cout << "[ERROR] cameraPointer->AnnounceFrame" << std::endl;
            return err;
        }

    }

    return VmbErrorSuccess;

}

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector& frames) {

    VmbErrorType err;

    for (FramePtrVector::iterator itr = frames.begin(); itr != frames.end(); itr++) {

        err = cameraPointer->QueueFrame(*itr);
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

    sys.GetCameras(cameras);
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

    for (CameraPtrVector::iterator itr = cameras.begin(); itr != cameras.end(); itr++) {
        CameraPtr cam_pointer = *itr;

        err = describeVimbaCamera(cam_pointer, cam_description);
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

SimpleFrameObserver::SimpleFrameObserver(CameraPtr cameraPointer) : IFrameObserver(cameraPointer) { }

void SimpleFrameObserver::FrameReceived(const FramePtr framePointer) {

    VmbFrameStatusType err_recv;
    VmbErrorType err;

    VmbUchar_t* image_buffer;

    if (framePointer->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        if (err_recv == VmbFrameStatusComplete) {
            std::cout << "Receive OK" << std::endl;

            err = framePointer->GetImage(image_buffer);
        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);
}
