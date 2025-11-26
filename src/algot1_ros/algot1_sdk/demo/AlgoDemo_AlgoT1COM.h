/**
 * @file: AlgoDemo_AlgoT1COM.h
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

class MyAlgoT1COMProject : public AlgoT1Project
{
public:
    MyAlgoT1COMProject() {}
    ~MyAlgoT1COMProject() {}
};

class MyAlgoT1DeviceCOM : public AlgoDeviceT1COM
{
private:
    AlgoT1Project *m_project = 0;

public:
    MyAlgoT1DeviceCOM() { m_project = 0; }
    MyAlgoT1DeviceCOM(AlgoT1Project *project) { m_project = project; }
    ~MyAlgoT1DeviceCOM() {}

public:
    void InspvaxaDataCallback(int handle, AlgoDataPacket *packet, char *data, int len)
    {
        if (packet != NULL)
        {
            AlgoINSPVAXADataPacket *p = ((AlgoINSPVAXADataPacket *)packet);
            printf("INSPVAXA:index=%d, pps=%.3f ms, yaw=%f, lon=%f, lat=%f, (deg)\n", p->index, p->cur_sec, p->yaw * RAD2DEG, p->posLon * RAD2DEG, p->posLat * RAD2DEG);
        }

        if (m_project && len > 0)
        {
            // printf("InspvaxaDataCallback:RawText,%d bytes\n", len);
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
            // printf("GpggaDataCallback:RawText,%d bytes.\n", len);
            m_project->saveGPGGAData(data, len);
        }
    }
};
