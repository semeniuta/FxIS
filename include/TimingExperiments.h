#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "TimeMeasure.h"
#include "ImageStream.h"

void csvStringFromTimestampsMatrix(TimestampsMatrix& timestamps, std::string &res);

bool compareTimestampsVectorsByFirstEntry(const std::vector<TimePoint>& a, const std::vector<TimePoint>& b);

void copyNewTimestamps(

        const TimestampsMatrix& timestamps,
        TimestampsMatrix& all_timestamps,
        int experiment_index

);

std::chrono::nanoseconds getRandomDuration(std::chrono::nanoseconds a, std::chrono::nanoseconds b);

void printTimestampsSnapshot(
        TimestampsMatrix& timestamps,
        TimePoint targetTime,
        unsigned long currentIndex,
        unsigned long targetIndex
);

void performImageStreamReadExperiment(
        ImageStream& im_stream_1,
        ImageStream& im_stream_2,
        unsigned int numReads,
        std::chrono::nanoseconds sleepInterval,
        std::chrono::nanoseconds sleepFluctuationMax,
        TimestampsMatrix& read_1_measurements,
        TimestampsMatrix& read_2_measurements,
        std::vector<TimestampsMatrix>& all_wts_1,
        std::vector<TimestampsMatrix>& all_wts_2,
        TimestampsMatrix& target_times_1,
        TimestampsMatrix& target_times_2,
        std::vector<cv::Mat>& images_1,
        std::vector<cv::Mat>& images_2,
        bool verbose
);

void createVectorOfUniqueWriteTimestamps(std::vector<TimestampsMatrix>& all_wts, TimestampsMatrix& res);

void saveCSV(const std::string& filename, const std::string& data, const std::string& header);

void saveImages(const std::vector<cv::Mat>& images, const std::string& suffix);

void saveTimestamps(
        TimestampsMatrix& timestamps,
        const std::string& filename,
        const std::string& header
);

int launchImageStreamReadExperiment(
        unsigned int streamSize,
        unsigned int numRequests,
        unsigned int tauSleep,
        unsigned int tauMaxFluctuation,
        bool verbose,
        bool doSaveImages
);

#endif
