#include "Serialize.h"
#include <sstream>
#include <string>

void csvStringFromTimestampsMatrix(TimestampsMatrix &timestamps, std::string &res) {

    std::stringstream buffer;

    auto num_rows = timestamps.size();
    auto num_cols = timestamps[0].size();

    for (int i = 0; i < num_rows; i++) {

        for (int j = 0; j < num_cols; j++) {

            buffer << timestamps[i][j].time_since_epoch().count();
            if (j < num_cols - 1) {
                buffer << ",";
            }

        }

        buffer << std::endl;

    }


    res = buffer.str();

}