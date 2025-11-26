#ifndef _ALGO_TURBO_JPEG_H_
#define _ALGO_TURBO_JPEG_H_

// not safe for multithread
class AlgoTurboJpeg
{
private:
    void *_jpegCompressor = 0; // tjhandle
    int TURBO_JPEG_QUALITY = 90;
    int TURBO_JPEG_COLOR_CHANNELS = 3;
    int _width = 0;
    int _height = 0;

    //!< Memory is allocated by tjCompress2 if _jpegSize == 0
    int _jpegSize = 0;
    char *_compressedImage = 0;

public:
    AlgoTurboJpeg();
    ~AlgoTurboJpeg();

    char *getJpegData();

    int getJpegSize();

    void initCompressor(int width, int height, int channels = 3, int quality = 80);

    void freeCompressor();

    bool compressJPEG(const char *pdata_raw);

    void releaseCompressJPEG();

    void decompressJPEG(char *pdata_jpeg, int jpegSize, char *pdata_raw, int rawSize);
};

#endif //!_ALGO_TURBO_JPEG_H_