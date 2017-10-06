#ifndef ARGSMANAGER_H
#define ARGSMANAGER_H

#include <string>
#include <vector>
#include <map>

class ArgsManager {

public:

    ArgsManager(int argc, char* argv[]);
    std::string getArg(unsigned int index);
    bool flagExists(std::string f);
    unsigned int getArgsBeforeFirstFlag(std::vector<std::string>& out);

private:

    void handleFlag(const std::string& arg, unsigned int index);

    int n_args;
    std::vector<std::string> args;
    std::map<std::string, unsigned int> flag_indices;

};


#endif