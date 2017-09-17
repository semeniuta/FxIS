#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "AVTVimba.h"
#include "AVTStreaming.h"
#include "AVTFrameObserverVideoStream.h"
#include "AVTFrameObserverImageStream.h"
#include "ImageStream.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Common/StreamSystemInfo.h"
#include "Common/ErrorCodeToMessage.h"

#include <opencv2/opencv.hpp>

const int N_FRAMES = 3;
const int CAMERA_INDEX = 0;

using namespace AVT::VmbAPI;

int main(int argc, char* argv[])
{

    VimbaSystem& sys = VimbaSystem::GetInstance();

    std::cout << "Vimba Version V" << sys << std::endl;

    VmbErrorType err;

    err = sys.Startup();
    if (err != VmbErrorSuccess) {
        std::cout << "Failed to start up. Shutting down\n";
        sys.Shutdown();
        return -1;
    }

    ImageStream image_stream(20);

    AVTStreaming avts(CAMERA_INDEX, N_FRAMES, image_stream);

    avts();

    sys.Shutdown();
    std::cout << "Shutting down normally\n";

    return 0;

}
