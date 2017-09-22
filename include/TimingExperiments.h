#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "TimeMeasure.h"
#include "ImageStream.h"

void csvStringFromTimestampsMatrix(TimestampsMatrix &timestamps, std::string &res);

std::chrono::nanoseconds getRandomDuration(std::chrono::nanoseconds a, std::chrono::nanoseconds b);

void performImageStreamReadExperiment(
        ImageStream& im_stream_1,
        ImageStream& im_stream_2,
        unsigned int numReads,
        std::chrono::nanoseconds sleepInterval,
        std::chrono::nanoseconds sleepFluctuationMax,
        std::string& csv_timestamps_1,
        std::string& csv_timestamps_2,
        std::string& csv_qspans_1,
        std::string& csv_qspans_2
);

#endif
