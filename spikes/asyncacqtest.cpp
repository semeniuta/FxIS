#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTFrameObserverVideoStream.h"
#include "DriverAVT/AVTFrameObserverImageStream.h"
#include "ImageStream.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

#include <opencv2/opencv.hpp>

const int N_FRAMES = 3;

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

    cam = cameras[0];
    std::map<std::string, VmbInt64_t> camera_features;

    err = openCameraWithImageFeatures(cam, camera_features);

    MatMaker mm(camera_features);

    unsigned int w, h;
    w = (unsigned int)camera_features["Width"];
    h = (unsigned int)camera_features["Height"];

    ImageStream image_stream(20, w, h, mm.getNumberOfChannels());

    IFrameObserverPtr observer(new AVTFrameObserverImageStream(cam, mm, image_stream));

    err = streamingStart(cam, frames, observer);

    std::cout<< "Press <enter> to stop acquisition...\n" ;
    getchar();

    err = acquisitionStop(cam);

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    return 0;

}
