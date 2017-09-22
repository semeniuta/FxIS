#include "TimingExperiments.h"
#include <sstream>
#include <string>
#include <thread>
#include <random>

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

//void saveCSV() {
//
//}

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
    std::string& csv_qspans_2
) {

    cv::Mat im_1;
    cv::Mat im_2;
    std::vector<std::vector<TimePoint>> timestamps_1;
    std::vector<std::vector<TimePoint>> timestamps_2;
    unsigned long index_1;
    unsigned long index_2;

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

        im_stream_1.getImage(t_now, im_1, timestamps_1, index_1, getim1_span);
        im_stream_2.getImage(t_now, im_2, timestamps_2, index_2, getim2_span);

        image1_query_spans.push_back({getim1_span.first, getim1_span.second});
        image2_query_spans.push_back({getim2_span.first, getim2_span.second});

    }

    csvStringFromTimestampsMatrix(timestamps_1, csv_timestamps_1);
    csvStringFromTimestampsMatrix(timestamps_2, csv_timestamps_2);
    csvStringFromTimestampsMatrix(image1_query_spans, csv_qspans_1);
    csvStringFromTimestampsMatrix(image2_query_spans, csv_qspans_2);

}