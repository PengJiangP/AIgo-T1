#include "AlgoSDK.h"
#include "AlgoTurbJpeg.h"
#include "AlgoOpenCv.h"

#ifdef USE_OPENCV

using namespace cv;

void AlgoOpenCv::createWindows(std::string name, int width, int height)
{
    cv::namedWindow(name.c_str(), cv::WINDOW_NORMAL);
    cv::resizeWindow(name.c_str(), width, height);
}

void AlgoOpenCv::showImage(std::string name, AlgoCameraDataPacket *packet)
{
    cv::Mat frame = cv::Mat::zeros(packet->imageHeight, packet->imageWidth, CV_8UC3);

    if (packet->imageType == 0)
    { // raw
        memcpy(frame.data, (unsigned char *)packet->pbuf, packet->pbufSize);
    }
    else
    {
#ifdef USE_TURBOJPEG
        // turbojpeg decode
        int rawLen = packet->imageHeight * packet->imageWidth * 3;
        char *rawData = new char[rawLen];
        AlgoTurboJpeg m_turbo;
        m_turbo.initCompressor(packet->imageWidth, packet->imageHeight);
        m_turbo.decompressJPEG(packet->pbuf, packet->pbufSize, rawData, rawLen);
        memcpy(frame.data, rawData, rawLen);
        delete[] rawData;
#endif
        // opencv decode
        std::vector<uint8_t> p_data;
        p_data.resize(packet->pbufSize);
        std::copy(packet->pbuf, packet->pbuf + packet->pbufSize, p_data.begin());
        frame = imdecode(p_data, cv::IMREAD_COLOR);
    }

    putText(frame, "cam" + std::to_string(packet->cameraId), Point(5, 20), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8, false);
    putText(frame, "id:" + std::to_string(packet->imageIndex), Point(5, 40), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8, false);
    putText(frame, "size:" + std::to_string(packet->imageWidth) + "*" + std::to_string(packet->imageHeight), Point(5, 60), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8, false);
    putText(frame, "cur_week:" + std::to_string(packet->cur_week), Point(5, 80), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8, false);
    putText(frame, "cur_sec:" + std::to_string(packet->cur_sec), Point(5, 100), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8, false);
    cv::imshow(name.c_str(), frame);
    cv::waitKey(10);
}

void AlgoOpenCv::toJpegImage(AlgoCameraDataPacket *packet, cv::Mat &frame)
{
#ifdef USE_TURBOJPEG
    // turbojpeg decode
    int rawLen = packet->imageHeight * packet->imageWidth * 3;
    char *rawData = new char[rawLen];
    AlgoTurboJpeg m_turbo;
    m_turbo.initCompressor(packet->imageWidth, packet->imageHeight);
    m_turbo.decompressJPEG(packet->pbuf, packet->pbufSize, rawData, rawLen);
    memcpy(frame.data, rawData, rawLen);
    delete[] rawData;
#endif
    // opencv decode
    std::vector<uint8_t> p_data;
    p_data.resize(packet->pbufSize);
    std::copy(packet->pbuf, packet->pbuf + packet->pbufSize, p_data.begin());
    frame = imdecode(p_data, cv::IMREAD_COLOR);
}

#endif //! USE_OPENCV