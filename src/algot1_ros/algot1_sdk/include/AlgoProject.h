/**
 * @file: AlgoProject.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of AlgoProject.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _ALGO_PROJECT_H_
#define _ALGO_PROJECT_H_

#include "AlgoData.h"
#include "AlgoDeviceT1.h"

#include <vector>

namespace Algo1010
{
        ///
        /// \class AlgoT1Project
        /// \brief This class will save AlgoDeviceAlgoT1 data using save file format in Device's sdcard.
        ///
        class AlgoT1Project : AlgoProject
        {
        public:
                // image
                std::string m_imageDataDirName0 = "cam0";
                std::string m_imageDataDescName0 = "description0.txt";
                std::string m_imageDataDirName1 = "cam1";
                std::string m_imageDataDescName1 = "description1.txt";

                std::string m_imageRootName = "";
                std::string m_curYmdDir = "";    // m_rootPath/yyyymmddd
                std::string m_curYmdHmsDir = ""; // m_rootPath/yyyymmddd/hhmmss
                std::string m_imageRootDir = ""; // m_rootPath/yyyymmddd/hhmmss/yyyymmddd_hhmmssimage

                std::string m_imageDataDir0 = ""; // m_rootPath/yyyymmddd/hhmmss/yyyymmddd_hhmmssimage/m_imageDataName
                std::string m_cam0DescPathName = "";
                std::string m_imageDataDir1 = ""; // m_rootPath/yyyymmddd/hhmmss/yyyymmddd_hhmmssimage/m_imageDataName
                std::string m_cam1DescPathName = "";

                // gnss+imu+fusion
                std::string m_gnss1PathName = "";
                std::string m_gnss2PathName = "";
                std::string m_memsPathName = "";
                std::string m_rtcmPathName = "";
                std::string m_ggaPathName = "";
                std::string m_rmcPathName = "";
                std::string m_inspvaxaPathName = "";
                std::string m_pppb2bPathName = "";

                // pos
                std::string m_posPathName = "";  //.pos
                std::string m_vinsPathName = ""; //$VINSPOS

        public:
                AlgoT1Project() {}
                ~AlgoT1Project() {}

                ///
                /// \brief create dirs and files for saveing device's data.
                ///
                virtual bool create(std::string rootPath = "");

                ///
                /// \brief open a exist project, open file and check dirs.
                ///
                virtual bool open(std::string projectPath, std::string projectName);

                ///
                /// \brief close a open project.
                ///
                virtual bool close();

                ///
                /// \brief save cam0 data. if device is bino, cam0 is right camera from the device's point of view.
                /// if device is mono, there is no cam0's callback data.
                ///
                virtual bool saveCam0Image(AlgoCameraDataPacket *frame, char *pData, int len); // 9000,default for ir

                ///
                /// \brief save cam1 data. if device is bino, cam1 is left camera from the device's point of view.
                /// if device is mono, cam1 will be run.
                ///
                virtual bool saveCam1Image(AlgoCameraDataPacket *frame, char *pData, int len); // 9001,default for rgb

                ///
                /// \brief save gnss1 data in raw format (unicorecom rangb). gnss1 is used for position usyally.
                ///
                virtual bool saveGnss1Data(char *pData, int len); // 9002, master, for pos

                ///
                /// \brief save gnss2 data in raw format (unicorecom rangb). gnss2 is used for calcute azimuth usyally.
                ///
                virtual bool saveGnss2Data(char *pData, int len); // 9003, slave, for yaw

                ///
                /// \brief save imu data. the data format refers to the file header.
                ///
                virtual bool saveMemsData(char *pData, int len); // 9004

                ///
                /// \brief save rtcm data. rtcm is used for rtk.
                ///
                virtual bool saveRTCMData(char *pData, int len); // 9005

                ///
                /// \brief save INSPVAXA data. it is fusion posion result, it's same with Noveltel and Beiyun's output data format.
                ///
                virtual bool saveINSPVAXAData(char *pData, int len); // 9006

                ///
                /// \brief save gpgga by nmea protocl. it is fusion posion result.
                ///
                virtual bool saveGPGGAData(char *pData, int len); // 9007

                ///
                /// \brief save rtcm data. rtcm is used for rtk.
                ///
                virtual bool savePPPB2BData(char *pData, int len); // 9008

                ///
                /// \brief save rtcm gprmc by nmea protocl. it's used for time sync usually.
                ///
                virtual bool saveGPRMCData(char *pData, int len); // 9009

                ///
                /// \brief save vinspos text.
                ///
                virtual bool saveVINSPOSData(char *pData, int len); // 9010
        };

}

#endif //!_ALGO_PROJECT_H_