#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "DriverAVT/AVTVimba.h"
#include "DriverAVT/AVTFrameObserverVideoStream.h"
#include "ImageStream.h"

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

    if (argc == 2) {

        int cam_index = atoi(argv[1]);

        if (cam_index < 0 || cam_index >= cameras.size()) {
            std::cout << "None-existent camera index provided(" << cam_index << "). Exiting." << std::endl;
            sys.Shutdown();
            return -1;
        }

        std::cout << "Opening camera with index " << cam_index << std::endl;
        cam = cameras[cam_index];

    } else {
        std::cout << "Opening the first camera" << std::endl;
        cam = cameras[0];
    }

    err = openCamera(cam, VmbAccessModeFull);
    if (err != VmbErrorSuccess) {
        std::cout << "Shutting down!\n";
        sys.Shutdown();
        return -1;
    }

    std::map<std::string, VmbInt64_t> camera_features;
    std::vector<std::string> feature_names = {"Height", "Width", "PixelFormat"};

    err = getFeaturesMap(cam, feature_names, camera_features);

    cv::namedWindow("Camera stream", cv::WINDOW_AUTOSIZE);
    MatMaker mm(camera_features);
    IFrameObserverPtr observer(new AVTFrameObserverVideoStream(cam, mm, "Camera stream"));

    err = streamingStart(cam, frames, observer);

    std::cout<< "Press <enter> to stop streaming...\n" ;
    getchar();

    err = acquisitionStop(cam);

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    return 0;

}
