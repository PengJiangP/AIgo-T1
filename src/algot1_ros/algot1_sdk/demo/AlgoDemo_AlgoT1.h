/**
 * @file: AlgoDemo_AlgoT1.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of an MyAlgoDevice and MyAlgoProject class.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#include "AlgoSDK.h"
#include "uliti/AlgoCoordTrans.h"

#include <iostream>
#include <thread>

using namespace Algo1010;

class MyAlgoT1Project : public AlgoT1Project
{
public:
    MyAlgoT1Project() {}
    ~MyAlgoT1Project() {}
};

class MyAlgoT1Device : public AlgoDeviceAlgoT1
{
private:
    AlgoT1Project *m_project = 0;

public:
    MyAlgoT1Device() { m_project = 0; }
    MyAlgoT1Device(AlgoT1Project *project) { m_project = project; }
    ~MyAlgoT1Device() {}

public:
    void Cam0DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len)
    {
        // opencv windows can't be showed, it must be showed in main thread!
        printf("CAM0:index=%d, pps=%.3f ms, jpegSize=%d bytes\n", packet->imageIndex, packet->cur_sec, len);

        if (m_project)
        {
            m_project->saveCam0Image(packet, data, len);
        }
    }

    void Cam1DataCallback(int handle, AlgoCameraDataPacket *packet, char *data, int len)
    {
        // opencv windows can't be showed, it must be showed in main thread!
        printf("CAM1:index=%d, pps=%.3f ms, jpegSize=%d bytes\n", packet->imageIndex, packet->cur_sec, len);

        if (m_project)
        {
            m_project->saveCam1Image(packet, data, len);
        }
    }

    void Gnss1DataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (m_project && len > 0)
        {
            m_project->saveGnss1Data(data, len);
        }
    }

    void Gnss2DataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (m_project && len > 0)
        {
            m_project->saveGnss2Data(data, len);
        }
    }

    void MemsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            AlgoImuDataPacket *p = ((AlgoImuDataPacket *)packet);
            printf("IMU:index=%d, week=%d,sec=%.3f s,tick_ms=%.1f ms\n", p->index, p->cur_week, p->cur_sec, p->curtick_ms);
        }

        if (m_project && len > 0)
        {
            m_project->saveMemsData(data, len);
        }
    }

    void RtcmDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {

        if (m_project && len > 0)
        {
            printf("RTCM:RawData %d bytes.\n", len);
            m_project->saveRTCMData(data, len);
        }
    }

    void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            AlgoINSPVAXADataPacket *p = ((AlgoINSPVAXADataPacket *)packet);
            printf("INSPVAXA:index=%d, pps=%.3f ms, yaw=%f, lon=%f, lat=%f, (deg)\n",
                   p->index, p->cur_sec, p->yaw * RAD2DEG, p->posLon * RAD2DEG, p->posLat * RAD2DEG);
        }

        if (m_project && len > 0)
        {
            m_project->saveINSPVAXAData(data, len);
        }
    }

    void GpggaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            printf("GPGGA:index=%d, pps=%.3f ms\n", ((AlgoGPGGADataPacket *)packet)->index, ((AlgoGPGGADataPacket *)packet)->cur_sec);
        }

        if (m_project && len > 0)
        {
            m_project->saveGPGGAData(data, len);
        }
    }

    void Pppb2bDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        printf("PPPB2B:RawData, %d bytys.\n", len);
        if (m_project && len > 0)
        {
            m_project->savePPPB2BData(data, len);
        }
    }

    void GprmcDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            AlgoGPRMCDataPacket *p = ((AlgoGPRMCDataPacket *)packet);
            printf("\n------------------GPRMC:index=%d, pps=%.3f ms, posType=%d, L=%.9f, B=%.9f\n",
                   p->index, p->cur_sec, p->posType, p->posLon, p->posLat);
        }

        if (m_project && len > 0)
        {
            m_project->saveGPRMCData(data, len);
        }
    }

    void WheelDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        printf("WheelSpeed:%d\n", len);
        if (m_project && len > 0)
        {
            // m_project->(data, len);
        }
    }

    void VinsDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            printf("VINS:index=%d, pps=%.3f ms\n", ((AlgoVINSPOSDataPacket *)packet)->index, ((AlgoVINSPOSDataPacket *)packet)->cur_sec);
        }

        if (m_project && len > 0)
        {
            m_project->saveVINSPOSData(data, len);
        }
    }
};
