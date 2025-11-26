/**
 * @file: AlgoLogger.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of AlgoOpenCv.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _ALGO_OPENCV_SHOW_H_
#define _ALGO_OPENCV_SHOW_H_

#include "AlgoData.h"

#ifdef USE_OPENCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
//#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>

using namespace Algo1010;

class AlgoOpenCv
{
public:
    static void createWindows(std::string name, int width = 640, int height = 360);
    static void showImage(std::string name, AlgoCameraDataPacket *packet);
    static void toJpegImage(AlgoCameraDataPacket *packet, cv::Mat &frame);
};
#endif

#endif //!_ALGO_OPENCV_SHOW_H_