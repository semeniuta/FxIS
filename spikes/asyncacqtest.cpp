#include <iostream>

#include "AVTVimba.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

const int N_FRAMES = 4;

using namespace AVT::VmbAPI;

int main(int argc, char* argv[])
{

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;
    CameraPtrVector cameras;
    FramePtrVector frames(N_FRAMES);
    CameraPtr cam;
    IFrameObserverPtr observer(new SimpleFrameObserver(cam));

    err = startupVimbaSystemAndGetCameras(sys, cameras);
    if (err != VmbErrorSuccess) {
        sys.Shutdown();
        return -1;
    }

    int n_cameras = cameras.size();
    std::cout << n_cameras << " cameras available" << std::endl;

    if (n_cameras == 0) {
        sys.Shutdown();
        return -1;
    }

    cam = cameras[0];

    err = cam->Open(VmbAccessModeFull);
    if (err != VmbErrorSuccess) {
        sys.Shutdown();
        return -1;
    }

    err = announceFrames(cam, frames, observer);

    err = cam->StartCapture();

    err = queueFrames(cam, frames);

    err = acquisitionStart(cam);

    // ...

    err = acquisitionStop(cam);

    sys.Shutdown();

    return 0;

}
