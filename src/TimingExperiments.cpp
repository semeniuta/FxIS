#include "TimingExperiments.h"
#include "ImageStreamRequester.h"
#include <sstream>
#include <string>
#include <thread>
#include <random>
#include <fstream>
#include <vector>
#include <algorithm>

void csvStringFromTimestampsMatrix(TimestampsMatrix &timestamps, std::string &res) {

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

        const TimestampsMatrix& timestamps,
        TimestampsMatrix& all_timestamps,
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

void createVectorOfUniqueWriteTimestamps(std::vector<TimestampsMatrix>& all_wts, TimestampsMatrix& res) {

    for (int i = 0; i < all_wts.size(); i++) {
        copyNewTimestamps(all_wts[i], res, i);
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

    std::random_device rd;

    std::default_random_engine generator{rd()};
    std::uniform_int_distribution<long long> distrib{a.count(), b.count()};
    return std::chrono::nanoseconds{distrib(generator)};

}

void printTimestampsSnapshot(
        TimestampsMatrix& timestamps,
        TimePoint targetTime,
        unsigned long currentIndex,
        unsigned long targetIndex
) {

    const unsigned int INDEX_OF_INTEREST = 0;

    for (int i = 0; i < timestamps.size(); i++) {

        auto this_time = timestamps[i][INDEX_OF_INTEREST];
        auto diff = targetTime - this_time;

        std::cout << i << "\t" << this_time.time_since_epoch().count() << "\t" << diff.count() << "\t";

        if (i == currentIndex) {
            std::cout << "<-";
        }

        if (i == targetIndex) {
            std::cout << "(closest)";
        }

        std::cout << std::endl;
    }

}

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
) {

    ImageResponse resp_1;
    ImageResponse resp_2;

    TimePoint t_now;

    ImageStreamRequester isr_1(im_stream_1);
    ImageStreamRequester isr_2(im_stream_2);

    isr_1.start();
    isr_2.start();

    for (int i = 0; i < numReads; i++) {

        std::chrono::nanoseconds random_fluctuation = getRandomDuration(-sleepFluctuationMax, sleepFluctuationMax);
        std::chrono::nanoseconds total_sleep = sleepInterval + random_fluctuation;

        std::this_thread::sleep_for(total_sleep);
        if (verbose) {
            std::cout << "Sleeping " << durationAsString(total_sleep) << std::endl;
        }

        t_now = currentTime();

        std::future<bool> future_1 = isr_1.requestImage(t_now);
        std::future<bool> future_2 = isr_2.requestImage(t_now);

        future_1.wait();
        isr_1.copyData(resp_1);

        future_2.wait();
        isr_2.copyData(resp_2);

        if (verbose) {
            std::cout << "===== Read #" << i << "=====" << std::endl;
            std::cout << "Camera 1" << std::endl;
            printTimestampsSnapshot(resp_1.timestamps_snapshot, t_now, resp_1.current_index, resp_1.target_index);
            std::cout << "Camera 2" << std::endl;
            printTimestampsSnapshot(resp_2.timestamps_snapshot, t_now, resp_2.current_index, resp_2.target_index);
            std::cout << std::endl;
        }

        target_times_1.push_back(resp_1.timestamps_snapshot[resp_1.target_index]);
        target_times_2.push_back(resp_2.timestamps_snapshot[resp_2.target_index]);

        std::sort(resp_1.timestamps_snapshot.begin(), resp_1.timestamps_snapshot.end(), compareTimestampsVectorsByFirstEntry);
        std::sort(resp_2.timestamps_snapshot.begin(), resp_2.timestamps_snapshot.end(), compareTimestampsVectorsByFirstEntry);

        all_wts_1.push_back(resp_1.timestamps_snapshot);
        all_wts_2.push_back(resp_2.timestamps_snapshot);

        resp_1.time_measurements.insert(resp_1.time_measurements.begin(), t_now);
        resp_2.time_measurements.insert(resp_2.time_measurements.begin(), t_now);

        read_1_measurements.push_back(resp_1.time_measurements);
        read_2_measurements.push_back(resp_2.time_measurements);

        images_1.push_back(resp_1.image.clone());
        images_2.push_back(resp_2.image.clone());

    }

}

void saveImages(const std::vector<cv::Mat>& images, const std::string& suffix) {

    for (int i = 0; i < images.size(); i++) {

        std::stringstream buff;
        buff << "image_" << suffix << "_" << i << ".png";

        cv::imwrite(buff.str(), images[i]);

    }

}

void saveTimestamps(
        TimestampsMatrix& timestamps,
        const std::string& filename,
        const std::string& header
) {

    std::string csv_str;

    csvStringFromTimestampsMatrix(timestamps, csv_str);
    saveCSV(filename, csv_str, header);

}
