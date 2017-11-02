#ifndef EXTENDEDIMAGESTREAM_H
#define EXTENDEDIMAGESTREAM_H

#include "ImageStream.h"

template <class T>
class ExtendedImageStream : public ImageStream {

public:

    explicit ExtendedImageStream(unsigned int size) : ImageStream(size) { }

    void init(uint width, uint height, uint numChannels) override;

    void storeImageData(cv::Mat image, T& data, TimePoint t);

    void getImage(
            TimePoint t,
            ImageResponse& out,
            T processing_result
    );

private:

    std::vector<T> processing_data;

};


#endif
