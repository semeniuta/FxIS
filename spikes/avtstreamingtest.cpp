#include <iostream>
#include "DriverAVT/AVTExperiments.h"
#include "ArgsManager.h"

const unsigned int REQUIRED_NUM_ARGS = 4;

int main(int argc, char* argv[]) {

    ArgsManager am(argc, argv);

    unsigned int stream_size = 20;
    unsigned int n_requests = 15;
    unsigned int tau_sleep = 180;
    unsigned int tau_mf = 50;
    bool verbose = false;
    bool do_save_images = false;

    unsigned int n_supplied_args;
    std::vector<std::string> supplied_args;
    n_supplied_args = am.getArgsBeforeFirstFlag(supplied_args);

    if (n_supplied_args == REQUIRED_NUM_ARGS) {

        stream_size = (unsigned int)std::stoi(supplied_args[0]);
        n_requests =  (unsigned int)std::stoi(supplied_args[1]);
        tau_sleep =   (unsigned int)std::stoi(supplied_args[2]);
        tau_mf =      (unsigned int)std::stoi(supplied_args[3]);

    }

    if (am.flagExists("--verbose")) {
        verbose = true;
    }

    if (am.flagExists("--save")) {
        do_save_images = true;
    }

    std::cout << "Launching stream reading experiment:" << std::endl;
    std::cout << "Stream size: " << stream_size << std::endl;
    std::cout << "Number of request: " << n_requests << std::endl;
    std::cout << "Sleep period: " << tau_sleep << std::endl;
    std::cout << "Max sleep fluctuation: " << tau_mf << std::endl;

    int retval = launchImageStreamReadExperiment(
            stream_size,
            n_requests,
            tau_sleep,
            tau_mf,
            verbose,
            do_save_images
    );

    return retval;

}
