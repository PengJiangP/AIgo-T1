#include "AlgoTurbJpeg.h"

#ifdef USE_TURBOJPEG
#include <turbojpeg.h>
#endif

AlgoTurboJpeg::AlgoTurboJpeg()
{
}

AlgoTurboJpeg::~AlgoTurboJpeg()
{
    freeCompressor();
}

char *AlgoTurboJpeg::getJpegData()
{
    return _compressedImage;
}

int AlgoTurboJpeg::getJpegSize()
{
    return _jpegSize;
}

void AlgoTurboJpeg::initCompressor(int width, int height, int channels, int quality)
{
#ifdef USE_TURBOJPEG
    _jpegCompressor = (void *)tjInitCompress();
    _width = width;
    _height = height;
    TURBO_JPEG_QUALITY = quality;
    TURBO_JPEG_COLOR_CHANNELS = channels;
#endif
}

void AlgoTurboJpeg::freeCompressor()
{
#ifdef USE_TURBOJPEG
    if (_jpegCompressor)
    {
        tjDestroy((tjhandle)_jpegCompressor);
        _jpegCompressor = NULL;
    }

    if (_compressedImage)
    {
        tjFree((unsigned char *)_compressedImage);
        _compressedImage = NULL;
    }
#endif
}

// compressJPEG
bool AlgoTurboJpeg::compressJPEG(const char *pdata_raw)
{
#ifdef USE_TURBOJPEG
    if (_jpegCompressor == NULL)
        return false;

    if (TURBO_JPEG_COLOR_CHANNELS == 1)
    {
        tjCompress2((tjhandle)_jpegCompressor, (unsigned char *)pdata_raw, _width, 0, _height, TJPF_BGR,
                    (unsigned char **)&_compressedImage, (unsigned long *)&_jpegSize, TJSAMP_GRAY, TURBO_JPEG_QUALITY, 0);
        return true;
    }

    if (TURBO_JPEG_COLOR_CHANNELS == 3)
    {
        tjCompress2((tjhandle)_jpegCompressor, (unsigned char *)pdata_raw, _width, 0, _height, TJPF_BGR,
                    (unsigned char **)&_compressedImage, (unsigned long *)&_jpegSize, TJSAMP_420, TURBO_JPEG_QUALITY, 0);
        return true;
    }

#endif

    return false;
}

void AlgoTurboJpeg::releaseCompressJPEG()
{
#ifdef USE_TURBOJPEG
    // to free the memory allocated by TurboJPEG (either by tjAlloc(),
    // or by the Compress/Decompress) after you are done working on it:
    if (_compressedImage != NULL)
    {
        tjFree((unsigned char *)_compressedImage);
        _compressedImage = NULL;
    }
#endif
}

// decompressJPEG
void AlgoTurboJpeg::decompressJPEG(char *pdata_jpeg, int jpegSize, char *pdata_raw, int rawSize)
{
#ifdef USE_TURBOJPEG
    // long unsigned int _jpegSize;     //!< _jpegSize from above
    // unsigned char *_compressedImage; //!< _compressedImage from above

    int jpegSubsamp;
    // int rawSize = _width * _height  * TURBO_JPEG_COLOR_CHANNELS;
    // unsigned char buffer[rawSize]; //!< will contain the decompressed image

    tjhandle _jpegDecompressor = tjInitDecompress();

    tjDecompressHeader2(_jpegDecompressor, (unsigned char *)pdata_jpeg, jpegSize, &_width, &_height, &jpegSubsamp);

    tjDecompress2(_jpegDecompressor, (unsigned char *)pdata_jpeg, jpegSize, (unsigned char *)pdata_raw, _width, 0, _height, TJPF_BGR, TJFLAG_FASTDCT);

    tjDestroy(_jpegDecompressor);
#endif
}