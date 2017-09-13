#include "AVTFrameObserverBasic.h"

AVTFrameObserverBasic::AVTFrameObserverBasic(CameraPtr cam) : IFrameObserver(cam) { }

void AVTFrameObserverBasic::FrameReceived(FramePtr frame) {

    VmbFrameStatusType err_recv;

    if (frame->GetReceiveStatus(err_recv) == VmbErrorSuccess) {

        if (err_recv == VmbFrameStatusComplete) {

            this->processFrame(frame);

        }

    } else {
        std::cout << "Receive unsuccessful" << std::endl;
    }

    this->m_pCamera->QueueFrame(frame);

}