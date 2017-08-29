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
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = feature_pointer->GetValue(tmp_result);
    if (err != VmbErrorSuccess) {
        return err;
    }
    result = tmp_result;

    return VmbErrorSuccess;
}

VmbErrorType announceFrames(CameraPtr cameraPointer, FramePtrVector frames, IFrameObserverPtr observer) {

    VmbErrorType err;
    VmbInt64_t payload_size;

    err = getFeatureValue(cameraPointer, "PayloadSize", payload_size);
    if (err != VmbErrorSuccess) {
        return err;
    }

    for (FramePtrVector::iterator itr = frames.begin(); itr != frames.end(); itr++) {

        FramePtr frame = *itr;
        frame.reset(new Frame(payload_size));

        err = frame->RegisterObserver(observer);
        if (err != VmbErrorSuccess) {
            return err;
        }

        err = cameraPointer->AnnounceFrame(frame);
        if (err != VmbErrorSuccess) {
            return err;
        }

    }

    return VmbErrorSuccess;

}

VmbErrorType queueFrames(CameraPtr cameraPointer, FramePtrVector frames) {

    VmbErrorType err;

    for (FramePtrVector::iterator itr = frames.begin(); itr != frames.end(); itr++) {

        err = cameraPointer->QueueFrame(*itr);
        if (err != VmbErrorSuccess) {
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
        return err;
    }

    err = start_feature->RunCommand();
    if (err != VmbErrorSuccess) {
        return err;
    }

    return VmbErrorSuccess;


}

VmbErrorType acquisitionStop(CameraPtr cameraPointer) {

    VmbErrorType err;
    FeaturePtr start_feature;

    err = cameraPointer->GetFeatureByName("AcquisitionStop", start_feature);
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = start_feature->RunCommand();
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = cameraPointer->EndCapture();
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = cameraPointer->FlushQueue();
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = cameraPointer->RevokeAllFrames();
    if (err != VmbErrorSuccess) {
        return err;
    }

    err = cameraPointer->Close();
    if (err != VmbErrorSuccess) {
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
        return err;
    }
    buffer << param << " ";

    err = cameraPointer->GetID(param);
    if (err != VmbErrorSuccess) {
        return err;
    }
    buffer << param << " ";

    buffer << std::endl;

    result =  buffer.str();
    return VmbErrorSuccess;
}

VmbErrorType describeVimbaSetup() {

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;
    CameraPtrVector cameras;
    std::string cam_description;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        return err;
    }

    sys.GetCameras(cameras);
    if (err != VmbErrorSuccess) {
        return err;
    }

    std::cout << cameras.size() << " cameras available:" << std::endl;

    for (CameraPtrVector::iterator itr = cameras.begin(); itr != cameras.end(); itr++) {
        CameraPtr cam_pointer = *itr;

        err = describeVimbaCamera(cam_pointer, cam_description);
        if (err != VmbErrorSuccess) {
            return err;
        }
        std::cout << cam_description;
    }

    sys.Shutdown();
}


SimpleFrameObserver::SimpleFrameObserver(CameraPtr cameraPointer) : IFrameObserver(cameraPointer) { }

void SimpleFrameObserver::FrameReceived(const FramePtr framePointer) {

    VmbFrameStatusType err_recv;

    if (framePointer->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        if (err_recv == VmbFrameStatusComplete) {
            std::cout << "Receive OK" << std::endl;
        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(framePointer);
}
