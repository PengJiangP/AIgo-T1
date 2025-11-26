/**
 * @file: algo_algot1_driver.h
 * @author: jack <283422622@qq.com>
 * @date: Nov.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of an MyAlgoDevice and MyAlgoProject class.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */
#ifndef ALGO_ALGOT1_DRIVER_H_
#define ALGO_ALGOT1_DRIVER_H_

#include "AlgoSDK.h"
#include "algo_algot1_driver.h"
#include "uliti/AlgoString.h"
#include "uliti/AlgoCoordTrans.h"

#include <iostream>
#include <thread>
#include <memory.h>
#include <iostream>
#include <thread>

namespace Algo1010
{
    class RosAlgoDeviceAlgoT1 : public AlgoDeviceAlgoT1
    {
    private:
        AlgoT1Project *m_project = 0;
        AlgoT1RosDriver *m_rosDriver = 0;
        bool print_log_raw_ = false;

    public:
        RosAlgoDeviceAlgoT1()
        {
            m_rosDriver = 0;
            m_project = 0;
        }
        RosAlgoDeviceAlgoT1(AlgoT1RosDriver *rosDriver, AlgoT1Project *project = 0)
        {
            m_rosDriver = rosDriver;
            m_project = project;
        }
        ~RosAlgoDeviceAlgoT1() {}

    public:
        void Cam0DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len)
        {
            // opencv windows can't be showed, it must be showed in main thread!
            if (len > 0 && print_log_raw_)
                printf("CAM0:index=%d, pps=%.3f ms, jpegSize=%d bytes\n", packet->imageIndex, packet->cur_sec, len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishCam0Image(packet, data, len);
            }

            if (m_project)
            {
                m_project->saveCam0Image(packet, data, len);
            }
        }

        void Cam1DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len)
        {
            // opencv windows can't be showed, it must be showed in main thread!
            if (len > 0 && print_log_raw_)
                printf("CAM1:index=%d, pps=%.3f ms, jpegSize=%d bytes\n", packet->imageIndex, packet->cur_sec, len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishCam1Image(packet, data, len);
            }

            if (m_project)
            {
                m_project->saveCam1Image(packet, data, len);
            }
        }

        void Cam2DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len)
        {
            // opencv windows can't be showed, it must be showed in main thread!
            if (len > 0 && print_log_raw_)
                printf("CAM2:index=%d, pps=%.3f ms, jpegSize=%d bytes\n", packet->imageIndex, packet->cur_sec, len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishCam2Image(packet, data, len);
            }
        }

        void Gnss1DataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("Gnss0DataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishGnss1Data(packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveGnss1Data(data, len);
            }
        }

        void Gnss2DataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("Gnss1DataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishGnss2Data(packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveGnss2Data(data, len);
            }
        }

        void MemsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
            {
                printf("MemsDataCallback:%d\n", len);
            }

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishMemsData((AlgoImuDataPacket *)packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveMemsData(data, len);
            }
        }

        void RtcmDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("RtcmDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishRtcmData(packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveRTCMData(data, len);
            }
        }

        void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
            {
                printf("InspvaxaDataCallback:%d\n", len);
            }

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishInspvaxaData((AlgoINSPVAXADataPacket *)packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveINSPVAXAData(data, len);
            }
        }

        void GpggaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
            {
                printf("GpggaDataCallback:%d\n", len);
            }

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishGpggaData((AlgoGPGGADataPacket *)packet, data, len);

                m_rosDriver->publishGnssState((AlgoGPGGADataPacket *)packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveGPGGAData(data, len);
            }
        }

        void Pppb2bDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("Pppb2bDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishPppb2bData(packet, data, len);
            }
        }

        void GprmcDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
            {
                printf("GprmcDataCallback:%d\n", len);
            }

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishGprmcData((AlgoGPRMCDataPacket *)packet, data, len);
            }
        }

        void WheelDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("WheelDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishWheelData(packet, data, len);
            }
        }

        void VinsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0 && print_log_raw_)
                printf("VinsDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishVinsData((AlgoVINSPOSDataPacket *)packet, data, len);
            }
        }
    };

    class RosAlgoDeviceAlgoT1COM : public AlgoDeviceT1COM
    {
    private:
        AlgoT1Project *m_project = 0;
        AlgoT1RosDriver *m_rosDriver = 0;

    public:
        RosAlgoDeviceAlgoT1COM()
        {
            m_rosDriver = 0;
            m_project = 0;
        }
        RosAlgoDeviceAlgoT1COM(AlgoT1RosDriver *rosDriver, AlgoT1Project *project = 0)
        {
            m_rosDriver = rosDriver;
            m_project = project;
        }
        ~RosAlgoDeviceAlgoT1COM() {}

    public:
        void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0)
                printf("InspvaxaDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishInspvaxaData((AlgoINSPVAXADataPacket *)packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveINSPVAXAData(data, len);
            }
        }

        void GpggaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
        {
            if (len > 0)
                printf("GpggaDataCallback:%d\n", len);

            if (m_rosDriver && packet != NULL)
            {
                m_rosDriver->publishGpggaData((AlgoGPGGADataPacket *)packet, data, len);
            }

            if (m_project && len > 0)
            {
                m_project->saveGPGGAData(data, len);
            }
        }
    };

}

#endif //! ALGO_ALGOT1_DRIVER_H_
