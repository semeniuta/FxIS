#include "ArgsManager.h"

ArgsManager::ArgsManager(int argc, char* argv[])


        std::string argument{argv[i]};

        this->args.push_back(argument);

        this->handleFlag(argument, i);

    }

}

void ArgsManager::handleFlag(const std::string& arg, unsigned int index) {

    if (arg[0] == '-') {
        this->flag_indices[arg] = index;
    }

}

std::string ArgsManager::getArg(unsigned int index) {

    if (index >= this->n_args) {
        throw std::range_error("Argument index is outside of the bounds");
    }

    return this->args[index];

}

bool ArgsManager::flagExists(std::string f) {

    if (this->flag_indices.find(f) == this->flag_indices.end()) {
        return false;
    }

    return true;

}

unsigned int ArgsManager::getArgsBeforeFirstFlag(std::vector<std::string>& out) {

    unsigned int i;
    for (i = 0; i < this->n_args; i++) {
        if (this->args[i][0] != '-') {
            out.push_back(this->args[i]);
        } else {
            break;
        }
    }

    return i;

}



