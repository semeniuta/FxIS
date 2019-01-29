/*
 * Launches AVTSimpleGrabService connected to two cameras and enables
 * a command interface in the main thread with possible commands "grab" and "exit".
 *
 */

#include "DriverAVT/AVTSimpleGrabService.h"

int main() {

    CamerasParameters cam_params = {
            {{"camera_index", 0}, {"n_frames", 3}},
            {{"camera_index", 1}, {"n_frames", 3}}
    };

    const int stream_size = 10;

    AVTSimpleGrabService srv;

    srv.init(stream_size, cam_params, false);
    srv.start();

    std::cout<< "Enter \"grab\" to grab images or \"exit\" to quit the program ...\n" ;

    std::string query;
    while (true) {

        std::cout << "> ";

        std::getline(std::cin, query);

        if (query == "grab") {

            auto resp = srv.grab();

        } else if (query == "exit") {
            break;
        } else {
            continue;
        }

    }
    srv.stop();

    return 0;



}