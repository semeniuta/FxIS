/*
 * Similarly to testsimplegrabservice,
 * launches AVTSimpleGrabService connected to two cameras and enables
 * a command interface in the main thread with possible commands "grab" and "exit".
 * In addition, on each grab, prints the difference between target timestamps
 * of the two cameras.
 *
 * An optional flag --stream enables video streaming.
 *
 */

#include "Service.h"
#include "DriverAVT/AVTSimpleGrabService.h"
#include "ArgsManager.h"

int main(int argc, char* argv[]) {

    ArgsManager am(argc, argv);

    const int STREAM_SIZE = 10;

    std::map<std::string, int> params_1 = {{"camera_index", 0}, {"n_frames", 3}};
    std::map<std::string, int> params_2 = {{"camera_index", 1}, {"n_frames", 3}};

    bool do_stream = am.flagExists("--stream");

    AVTSimpleGrabService srv;
    srv.init(
            STREAM_SIZE,
            {params_1, params_2},
            do_stream
    );

    srv.start();

    std::cout<< "Enter \"grab\" to grab images or \"exit\" to quit the program ...\n" ;

    std::string query;
    unsigned int grab_idx = 0;
    while (true) {

        std::cout << "> ";

        std::getline(std::cin, query);

        if (query == "grab") {

            std::vector<ImageResponse> responses = srv.grab();

            auto idx_0 = responses[0].target_index;
            auto idx_1 = responses[1].target_index;

            std::cout << durationAsString(
                responses[0].timestamps_snapshot[idx_0][0] - responses[1].timestamps_snapshot[idx_1][0]
            ) << std::endl;


        } else if (query == "exit") {
            break;
        } else {
            continue;
        }

    }
    srv.stop();

    return 0;
}
