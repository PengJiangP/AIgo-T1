#ifndef _AlgoDeviceAlgoT1_Private_H_
#define _AlgoDeviceAlgoT1_Private_H_

#include "AlgoSDK.h"

#include "uliti/AlgoStreamBase.h"
#include "uliti/AlgoParserBase.h"
#include "uliti/AlgoThreadBase.h"

#include "parser/AlgoImageParser.h"
#include "parser/AlgoTextParser.h"

#include <functional>
#include <mutex>

namespace Algo1010
{

    class AlgoStreamBaseThread : public AlgoThreadBase
    {
    private:
        unsigned char m_buffer[ALGO_DAT_MAX_LEN];
        std::mutex mutex;

    public:
        AlgoStreamBase *m_stream = NULL;
        AlgoParserBase *m_parser = NULL;
        AlgoDataPacket *m_packet = NULL;
        AlgoT1DataType m_dataType = AlgoT1DataType::ALGO_DAT_NONE;

    public:
        AlgoStreamBaseThread(void *obj) { object_ = obj; }
        ~AlgoStreamBaseThread() {}

        void run() override
        {
            try
            {
                if (m_stream == NULL)
                {
                    std::cerr << "Not int m_pStream, thread exit" << std::endl;
                    return;
                } 

                if (!m_stream->open())
                {
                    std::cerr << "Failed to open." << std::endl;
                }
                else
                {
                    // write start cmd
                    int sendlen = m_stream->write(m_stream->m_startCmd, m_stream->m_startCmdLen);
                    std::cout << "Open OK, Thread is running. (" << m_stream->getinfo() << ")" << std::endl;
                }

                AlgoDeviceAlgoT1 *m_device = (AlgoDeviceAlgoT1 *)object_;
                while (isRunning_)
                {
                    // Binary
                    memset(m_buffer, 0, ALGO_DAT_MAX_LEN);
                    int len = m_stream->read(m_buffer, ALGO_DAT_MAX_LEN);
                    // std::cout << m_stream->getinfo() << ":read " << len << " bytes." << std::endl;

                    if (len == 0)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }
                    else
                    {
                        if (m_dataType == ALGO_DAT_CAM0 || m_dataType == ALGO_DAT_CAM1)
                        {
                            std::unique_lock<std::mutex> lock(mutex);
                            if (m_parser->process((char *)m_buffer, len))
                            {
                                m_packet = ((AlgoImageParser *)m_parser)->m_frame;
                                m_device->dataCallback(0, m_dataType, m_packet,
                                                       (char *)((AlgoCameraDataPacket *)m_packet)->pbuf, ((AlgoCameraDataPacket *)m_packet)->pbufSize);
                            }
                        }
                        else if (m_dataType == ALGO_DAT_MEMS)
                        {
                            std::unique_lock<std::mutex> lock(mutex);
                            m_device->dataCallback(0, m_dataType, NULL, (char *)m_buffer, len);

                            if (m_parser != NULL && m_parser->process((char *)m_buffer, len))
                            {
                                std::vector<AlgoDataPacketPtr> packet = ((AlgoTextParser *)m_parser)->popPacket();
                                for (size_t i = 0; i < packet.size(); i++)
                                {
                                    m_device->dataCallback(0, m_dataType, packet[i].get(), NULL, 0);
                                }
                            }
                        }
                        else
                        {
                            std::unique_lock<std::mutex> lock(mutex);
                            m_device->dataCallback(0, m_dataType, NULL, (char *)m_buffer, len);

                            if (m_parser != NULL && m_parser->process((char *)m_buffer, len))
                            {
                                std::vector<AlgoDataPacketPtr> packet = ((AlgoTextParser *)m_parser)->popPacket();
                                for (size_t i = 0; i < packet.size(); i++)
                                {
                                    m_device->dataCallback(0, m_dataType, packet[i].get(), NULL, 0);
                                }
                            }
                        }
                    }
                }
                m_stream->close();
                std::cout << "StreamThread exit." << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "StreamThread exit. exception:" << std::string(e.what()) << std::endl;
            }
        }
    };

    class AlgoDeviceAlgoT1_Private
    {
    private:
        AlgoDeviceAlgoT1 *m_device = 0;

    public:
        AlgoStreamBase m_streamCam[2]; // net_cam1_settings&net_cam1_settings
        AlgoStreamBase m_streamNet[8]; // net_1_settings ... net_8_settings
        AlgoStreamBase m_streamVins;

        AlgoStreamBaseThread *m_threadCam[2];
        AlgoStreamBaseThread *m_threadNet[8];
        AlgoStreamBaseThread *m_threadVins;

    public:
        AlgoDeviceAlgoT1_Private(AlgoDeviceAlgoT1 *device);
        ~AlgoDeviceAlgoT1_Private();
    };

    AlgoDeviceAlgoT1_Private::AlgoDeviceAlgoT1_Private(AlgoDeviceAlgoT1 *device)
    {
        m_device = device;

        // new thread
        m_threadCam[0] = new AlgoStreamBaseThread(m_device);
        m_threadCam[1] = new AlgoStreamBaseThread(m_device);
        for (int netId = 0; netId < 8; netId++)
        {
            m_threadNet[netId] = new AlgoStreamBaseThread(m_device);
        }
        m_threadVins = new AlgoStreamBaseThread(m_device);

        m_threadCam[0]->m_dataType = AlgoT1DataType::ALGO_DAT_CAM0;
        m_threadCam[1]->m_dataType = AlgoT1DataType::ALGO_DAT_CAM1;
        m_threadNet[0]->m_dataType = AlgoT1DataType::ALGO_DAT_GNSS1;
        m_threadNet[1]->m_dataType = AlgoT1DataType::ALGO_DAT_GNSS2;
        m_threadNet[2]->m_dataType = AlgoT1DataType::ALGO_DAT_MEMS;
        m_threadNet[3]->m_dataType = AlgoT1DataType::ALGO_DAT_RTCM;
        m_threadNet[4]->m_dataType = AlgoT1DataType::ALGO_DAT_INSPVAXA;
        m_threadNet[5]->m_dataType = AlgoT1DataType::ALGO_DAT_GPGGA;
        m_threadNet[6]->m_dataType = AlgoT1DataType::ALGO_DAT_PPPB2B;
        m_threadNet[7]->m_dataType = AlgoT1DataType::ALGO_DAT_GPRMC;
        m_threadVins->m_dataType = AlgoT1DataType::ALGO_DAT_VINS;

        // new stream
        m_threadCam[0]->m_stream = &m_streamCam[0];
        m_threadCam[1]->m_stream = &m_streamCam[1];
        for (int netId = 0; netId < 8; netId++)
        {
            m_threadNet[netId]->m_stream = &m_streamNet[netId];
        }
        m_threadVins->m_stream = &m_streamVins;

        // new parser
        m_threadCam[0]->m_parser = new AlgoImageParser();
        m_threadCam[1]->m_parser = new AlgoImageParser();

        // m_threadNet[0]->m_parser = new AlgoTextParser();
        // m_threadNet[1]->m_parser = new AlgoTextParser();
        m_threadNet[2]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_ALGOIMU); // support both $ALGOIMU and $1010IMU
        // m_threadNet[3]->m_parser = new AlgoTextParser();
        m_threadNet[4]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_INSPVAXA);
        m_threadNet[5]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_NMEA_GPGGA);
        // m_threadNet[6]->m_parser = new AlgoTextParser();
        m_threadNet[7]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_NMEA_GPRMC);
        m_threadVins->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_ALGOVINS);
    }

    AlgoDeviceAlgoT1_Private::~AlgoDeviceAlgoT1_Private()
    {
        // delete thread
        delete m_threadCam[0];
        delete m_threadCam[1];
        for (int netId = 0; netId < 8; netId++)
        {
            delete m_threadNet[netId];
        }
        delete m_threadVins;

        // delete parser
        delete m_threadCam[0]->m_parser;
        delete m_threadCam[1]->m_parser;
        for (int netId = 0; netId < 8; netId++)
        {
            if (m_threadNet[netId]->m_parser != NULL)
                delete m_threadNet[netId]->m_parser;
        }
        delete m_threadVins->m_parser;

        m_device = 0;
    }

}

#endif //!_AlgoDeviceAlgoT1_Private_H_