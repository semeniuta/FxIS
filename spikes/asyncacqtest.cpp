#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "AVTVimba.h"
#include "SimpleObserver.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

#include <opencv2/opencv.hpp>

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

    err = startupVimbaSystemAndGetCameras(sys, cameras);
    if (err != VmbErrorSuccess) {
        std::cout << "Shutting down!\n";
        sys.Shutdown();
        return -1;
    }

    unsigned long n_cameras = cameras.size();
    std::cout << n_cameras << " cameras available" << std::endl;

    if (n_cameras == 0) {
        std::cout << "Shutting down!\n";
        sys.Shutdown();
        return -1;
    }

    //cam = cameras[0];
    cam = cameras[1];

    err = openCamera(cam, VmbAccessModeFull);
    if (err != VmbErrorSuccess) {
        std::cout << "Shutting down!\n";
        sys.Shutdown();
        return -1;
    }

    VmbInt64_t height, width, pixelFormat;

    std::map<std::string, VmbInt64_t> camera_features;
    std::vector<std::string> feature_names = {"Height", "Width", "PixelFormat"};

    err = getFeaturesMap(cam, feature_names, camera_features);

    MatMaker mm(camera_features);
    IFrameObserverPtr observer(new SimpleFrameObserver(cam, mm));

    err = announceFrames(cam, frames, observer);

    err = cam->StartCapture();

    err = queueFrames(cam, frames);

    err = acquisitionStart(cam);

    std::cout<< "Press <enter> to stop acquisition...\n" ;
    getchar();

    err = acquisitionStop(cam);

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    return 0;

}
