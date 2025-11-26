#ifndef _AlgoDeviceT1COM_Private_H_
#define _AlgoDeviceT1COM_Private_H_

#include "AlgoSDK.h"
#include "AlgoDeviceT1COM.h"

#include "uliti/AlgoStreamBase.h"
#include "uliti/AlgoParserBase.h"
#include "uliti/AlgoThreadBase.h"
#include "parser/AlgoImageParser.h"
#include "parser/AlgoTextParser.h"

#include <functional>
#include <mutex>

#define SERIAL_COM_NUM 2

namespace Algo1010
{

    class AlgoStreamBaseThreadCOM : public AlgoThreadBase
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
        AlgoStreamBaseThreadCOM(void *obj) { object_ = obj; }
        ~AlgoStreamBaseThreadCOM() {}

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

                AlgoDeviceT1COM *m_device = (AlgoDeviceT1COM *)object_;
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
                        if (m_dataType == ALGO_DAT_GPGGA)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(40));
                        }

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

    class AlgoDeviceT1COM_Private
    {
    private:
        AlgoDeviceT1COM *m_device = 0;

    public:
        AlgoStreamBase m_streamCom[8];
        AlgoStreamBaseThreadCOM *m_threadCom[8];

        AlgoDeviceT1COM_Private(AlgoDeviceT1COM *device)
        {
            m_device = device;

            for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
            {
                m_threadCom[netId] = new AlgoStreamBaseThreadCOM(m_device);
            }

            for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
            {
                m_threadCom[netId]->m_stream = &m_streamCom[netId];
            }

            m_threadCom[0]->m_dataType = AlgoT1DataType::ALGO_DAT_INSPVAXA;
            m_threadCom[1]->m_dataType = AlgoT1DataType::ALGO_DAT_GPGGA;
            m_threadCom[0]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_INSPVAXA);
            m_threadCom[1]->m_parser = new AlgoTextParser(AlgoTextDataType::ALGO_NMEA_GPGGA);
        }

        ~AlgoDeviceT1COM_Private()
        {
            for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
            {
                if (m_threadCom[netId]->m_parser != NULL)
                    delete m_threadCom[netId]->m_parser;
            }

            for (int netId = 0; netId < SERIAL_COM_NUM; netId++)
            {
                delete m_threadCom[netId];
            }

            m_device = 0;
        }
    };

}

#endif //!_AlgoDeviceT1COM_Private_H_