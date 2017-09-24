#include "TimingExperiments.h"
#include <sstream>
#include <string>
#include <thread>
#include <random>
#include <fstream>
#include <vector>
#include <algorithm>

void csvStringFromTimestampsMatrix(TimestampsMatrix &timestamps, unsigned long ci, std::string &res) {

    std::stringstream buffer;

    auto num_rows = timestamps.size();
    auto num_cols = timestamps[0].size();

    for (int i = 0; i < num_rows; i++) {

        for (int j = 0; j < num_cols; j++) {

            TimePoint tp = timestamps[i][j];
            buffer << tp.time_since_epoch().count();
            if (j < num_cols - 1) {
                buffer << ",";
            }

        }

        buffer << std::endl;

    }


    res = buffer.str();

}

bool compareTimestampsVectorsByFirstEntry(const std::vector<TimePoint>& a, const std::vector<TimePoint>& b) {
    return (a[0] < b[0]);
}

void copyNewTimestamps(

        const TimestampsMatrix & timestamps,
        TimestampsMatrix & all_timestamps,
        int experiment_index

) {

    int start_with = 0;

    if (experiment_index > 0) {

        for (int j = 0; j < timestamps.size(); j++) {

            std::vector<TimePoint> to_find = timestamps[j];

            if (!std::binary_search(all_timestamps.begin(), all_timestamps.end(), to_find)) {
                start_with = j;
                break;
            }

        }

    }

    for (int j = start_with; j < timestamps.size(); j++) {
        auto ts = timestamps[j];
        all_timestamps.push_back(ts);
    }

}

void saveCSV(const std::string& filename, const std::string& data, const std::string& header) {

    std::ofstream f;
    f.open(filename);

    f << header << std::endl;
    f << data;

    f.close();

}

std::chrono::nanoseconds getRandomDuration(std::chrono::nanoseconds a, std::chrono::nanoseconds b) {

    std::default_random_engine generator;
    std::uniform_int_distribution<long long> distrib{a.count(), b.count()};
    return std::chrono::nanoseconds{distrib(generator)};

}

void performImageStreamReadExperiment(
    ImageStream& im_stream_1,
    ImageStream& im_stream_2,
    unsigned int numReads,
    std::chrono::nanoseconds sleepInterval,
    std::chrono::nanoseconds sleepFluctuationMax,
    std::string& csv_timestamps_1,
    std::string& csv_timestamps_2,
    std::string& csv_qspans_1,
    std::string& csv_qspans_2,
    std::vector<cv::Mat>& images_1,
    std::vector<cv::Mat>& images_2
) {

    cv::Mat im_1;
    cv::Mat im_2;
    TimestampsMatrix timestamps_1;
    TimestampsMatrix timestamps_2;
    unsigned long index_1, index_2, current_index_1, current_index_2;

    TimestampsMatrix all_timestamps_1;
    TimestampsMatrix all_timestamps_2;

    TimestampsMatrix image1_query_spans;
    TimestampsMatrix image2_query_spans;

    for (int i = 0; i < numReads; i++) {

        std::chrono::nanoseconds random_fluctuation = getRandomDuration(-sleepFluctuationMax, sleepFluctuationMax);
        std::chrono::nanoseconds total_sleep = sleepInterval + random_fluctuation;

        std::this_thread::sleep_for(total_sleep);

        TimePoint t_now = currentTime();

        TimePoint t_im_1;
        TimePoint t_im_2;
        TimePointsPair getim1_span;
        TimePointsPair getim2_span;

        im_stream_1.getImage(t_now, im_1, timestamps_1, index_1, current_index_1, getim1_span);
        im_stream_2.getImage(t_now, im_2, timestamps_2, index_2, current_index_2, getim2_span);

        std::sort(timestamps_1.begin(), timestamps_1.end(), compareTimestampsVectorsByFirstEntry);
        std::sort(timestamps_2.begin(), timestamps_2.end(), compareTimestampsVectorsByFirstEntry);

        copyNewTimestamps(timestamps_1, all_timestamps_1, i);
        copyNewTimestamps(timestamps_2, all_timestamps_2, i);

        image1_query_spans.push_back({getim1_span.first, getim1_span.second});
        image2_query_spans.push_back({getim2_span.first, getim2_span.second});

        images_1.push_back(im_1.clone());
        images_2.push_back(im_2.clone());

    }

    csvStringFromTimestampsMatrix(all_timestamps_1, current_index_1, csv_timestamps_1);
    csvStringFromTimestampsMatrix(all_timestamps_2, current_index_2, csv_timestamps_2);
    csvStringFromTimestampsMatrix(image1_query_spans, current_index_1, csv_qspans_1);
    csvStringFromTimestampsMatrix(image2_query_spans, current_index_2, csv_qspans_2);

}