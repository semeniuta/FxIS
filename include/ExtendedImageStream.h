#ifndef EXTENDEDIMAGESTREAM_H
#define EXTENDEDIMAGESTREAM_H

#include "ImageStream.h"

template <class T>
class ExtendedImageStream : public ImageStream {

public:

    explicit ExtendedImageStream(unsigned int size) : ImageStream(size) { }

    void init(uint width, uint height, uint numChannels) override;

    void storeImageData(unsigned char* imageDataPtr, TimePoint t) override = delete;
    void storeImageData(unsigned char* imageDataPtr, T& data, TimePoint t);

private:

    std::vector<T> processing_data;

};


#endif
