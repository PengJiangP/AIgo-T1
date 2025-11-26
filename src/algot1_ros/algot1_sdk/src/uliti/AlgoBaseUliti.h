/**
 * @file: AlgoBaseUliti.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of function using AlgoBase libaray.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _ALGOBASE_ULITI_H_
#define _ALGOBASE_ULITI_H_

#include "AlgoProject.h"

#include "uliti/AlgoSystemTime.h"
#include "uliti/AlgoDirectory.h"

#include "stream/rtktime.h"

#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>

using namespace RTKLIB;

namespace Algo1010
{

    std::string genImagePathName(AlgoT1Project *project, AlgoCameraDataPacket *image, bool relative)
    {
        std::string imagedatadir = "";
        if (image->cameraId == 0)
            imagedatadir = project->m_imageDataDir0;
        else
            imagedatadir = project->m_imageDataDir1;

        char subFolderName[64];
        sprintf(subFolderName, "%04d", image->imageIndex / 10000);
        std::string curImageDataDir = imagedatadir + "/" + std::string(subFolderName);

        char buffer[256];
        memset(buffer, 0, 256);
        sprintf(buffer, "%010d%010d.jpg", image->imageIndex, (int)(image->cur_sec * 1000));

        std::string relativeImagePathName = "cam" + std::to_string(image->cameraId) + "/" + std::string(subFolderName) + "/" + std::string(buffer);

        if (relative)
        {
            return relativeImagePathName;
        }
        else
        {
            // create new folder
            if (access(curImageDataDir.c_str(), F_OK) != 0)
            {
                AlgoDirectory::createDirectory(curImageDataDir);
            }

            std::string imagePathName = curImageDataDir + "/" + std::string(buffer);
            return imagePathName;
        }
    }

    std::string genImageDesc(AlgoT1Project *project, AlgoCameraDataPacket *image)
    {
        // update description
        if (image)
        {
            std::string imagePathName = genImagePathName(project, image, true);

            gtime_t gpst = gpst2time(image->cur_week, image->cur_sec);
            gtime_t utct = gpst2utc(gpst);
            utct.time += 8 * 3600.0;

            char timeBuff[128] = {0};
            memset(timeBuff, 0, 128);
            time2str(utct, timeBuff, 3);

            char lineBuffer[512] = {0};
            memset(lineBuffer, 0, 512);
            sprintf(lineBuffer, "%s %d %.3f #%s,%d,%.1f,%.1f,%.1f,%.1f#\n",
                    imagePathName.c_str(), image->cur_week, image->cur_sec,
                    timeBuff,
                    image->hasPPS, // reserved
                    image->dpps_sec,
                    image->pps_sec,
                    image->ppstick_ms,
                    image->curtick_ms);

            std::string lineString(lineBuffer);
            return lineString;
        }

        return "";
    }

    void WriteBMP(char *pImg, int w, int h, const char *filename)
    {
        // 计算每行所占字节数
        // +3是怕出现不满足4的倍数这种情况
        // /4*4的目的是保证结果为4的倍数
        int l = (w * 3 + 3) / 4 * 4;

        // 位图头文件信息（不包含BMP标志）+信息数据
        // 利用或运算巧妙的将图像面数及像素位数合并
        int bmi[] = {l * h + 54, 0, 54, 40, w, h, 1 | ((3 * 8) << 16), 0, 0, 0, 0, 0, 0};
        // 创建/打开文件
        FILE *fp = fopen(filename, "wb");
        // 写入BMP标志
        fprintf(fp, "BM");
        // 写入位图头文件信息+信息数据
        fwrite(&bmi, 52, 1, fp);

        // 写入位图数据
        // fwrite(pImg, 1, l * h, fp);
        for (int j = h - 1; j >= 0; j--)
        {
            fwrite((pImg + j * 3 * w), w * 3, 1, fp);
        }

        fclose(fp);
    }
}

#endif //!_ALGOBASE_ULITI_H_