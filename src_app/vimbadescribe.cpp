/*
 * Describe the current Vimba setup in the terminal.
 *
 * Uses describeVimbaSetup function from DriverAVT/AVTVimba.h
 *
 */

#include <iostream>

#include "DriverAVT/AVTVimba.h"

int main( int argc, char* argv[] ) {

    if (describeVimbaSetup() != VmbErrorSuccess) {
        std::cout << "Some problem occured" << std::endl;
        return -1;
    }

    return 0;

}
