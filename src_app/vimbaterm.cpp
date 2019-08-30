/*
 *
 * Connect to all available Allied Vision cameras and launch
 * a command interface in the main thread with possible commands "grab" and "exit".
 * On each grab, the differences between the target timestamps of cameras
 * relative to the first camera are printed.
 *
 * An optional flag --stream enables video streaming.
 *
 */

#include <iostream>
#include "DriverAVT/AVTSimpleGrabService.h"
#include "DriverAVT/AVTVimba.h"
#include "Service.h"
#include "ArgsManager.h"


int main(int argc, char* argv[]) {

    const int STREAM_SIZE = 10;
    const int N_FRAMES = 3;

    VimbaSystem& sys = VimbaSystem::GetInstance();

    CameraPtrVector cameras;

    VmbErrorType err;
    err = startupVimbaSystemAndGetCameras(sys, cameras);
    if (err != VmbErrorSuccess) {
        std::cerr << "Could not start Vimba API\n";
        sys.Shutdown();
        return -1;
    }

    auto n_cameras = cameras.size();

    CamerasParameters params{};
    for (size_t i = 0; i < n_cameras; i++) {
        params.push_back( {{"camera_index", i}, {"n_frames", N_FRAMES}} );
    }

    ArgsManager am{argc, argv};
    bool do_stream = am.flagExists("--stream");

    AVTSimpleGrabService srv{};
    srv.enableManualVimbaControl();
    srv.init(STREAM_SIZE, params, do_stream);
    srv.start();

    std::cout<< "Enter \"grab\" to grab images or \"exit\" to quit the program ...\n" ;

    std::string query;
    while (true) {

        std::cout << "> ";

        std::getline(std::cin, query);

        if (query == "grab") {

            std::vector<ImageResponse> responses = srv.grab();

            std::cout << "Grabbed\n";

            if (n_cameras > 1) {

                std::vector<unsigned long> target_indices;
                for (int cam_i = 0; cam_i < n_cameras; cam_i++) {
                    target_indices.push_back(responses[cam_i].target_index);
                }

                // Base timestamp (the 1st camera) to compare other with
                auto t0 = responses[0].timestamps_snapshot[target_indices[0]][0];

                std::cout << "Target timestamp differences:\n";
                for (int cam_i = 1; cam_i < n_cameras; cam_i++) {

                    auto target_idx = target_indices[cam_i];
                    auto target_t = responses[cam_i].timestamps_snapshot[target_idx][0];
                    auto t_diff = target_t - t0;

                    std::cout << durationAsString(t_diff) << " ";
                }

                std::cout << std::endl;

            }

        } else if (query == "exit") {
            break;
        } else {
            continue;
        }

    }

    srv.stop();
    sys.Shutdown();

    return 0;
}
