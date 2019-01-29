/*
 * Demonstrates functionality of ArgsManager
 */

#include <iostream>
#include <vector>
#include <string>
#include "ArgsManager.h"

int main(int argc, char* argv[]) {

    ArgsManager am(argc, argv);

    std::cout << "The supplied arguments:" << std::endl;

    for (unsigned int i = 0; i < argc; i++) {
        std::cout << am.getArg(i) << std::endl;
    }

    std::vector<std::string> args_before_flags;
    unsigned int n_args_before_flags = am.getArgsBeforeFirstFlag(args_before_flags);

    std::cout << "Number of arguments before the first flag: " << args_before_flags.size();
    std::cout << ", which are: ";
    for (unsigned int i = 0; i < n_args_before_flags; i++) {
        std::cout << args_before_flags[i];
        if (i < n_args_before_flags - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }

}