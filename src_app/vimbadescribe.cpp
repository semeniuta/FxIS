#include <iostream>

#include "AVTVimba.h"

int main( int argc, char* argv[] )
{

    if (describeVimbaSetup() != VmbErrorSuccess) {
        std::cout << "Some problem occured" << std::endl;
        return -1;
    }

    return 0;

}
