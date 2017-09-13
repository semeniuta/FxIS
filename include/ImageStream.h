#ifndef IMAGESTREAM_H
#define IMAGESTREAM_H

#include <vector>
#include <mutex>
#include <opencv2/opencv.hpp>

class ImageStream {

public:
    ImageStream(uint size, uint width, uint height, uint numChannels);
    int storeImageData(unsigned char* imageDataPtr);
    int getImage(uint index, cv::Mat out);

private:
    unsigned int stream_size;
    unsigned int w;
    unsigned int h;
    unsigned int num_channels;
    unsigned int current_index;
    std::vector<cv::Mat> images;
    std::mutex mutex;

};

#endif
