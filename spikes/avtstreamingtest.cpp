#include <iostream>
#include "DriverAVT/AVTExperiments.h"

int main(int argc, char* argv[]) {

    int retval;

    unsigned int stream_size = 20;
    unsigned int n_requests = 15;
    unsigned int tau_sleep = 180;
    unsigned int tau_mf = 50;
    bool verbose = true;
    bool do_save_images = false;

    retval = launchImageStreamReadExperiment(
            stream_size,
            n_requests,
            tau_sleep,
            tau_mf,
            verbose,
            do_save_images
    );

    return retval;

}
