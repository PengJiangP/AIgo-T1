#ifndef _AlgoImageParser_H_
#define _AlgoImageParser_H_

#include "AlgoData.h"
#include "uliti/AlgoParserBase.h"

#include <string.h>
#include <iostream>

using namespace Algo1010;

class AlgoImageParser : public AlgoParserBase
{
public:
    // result data
    AlgoCameraDataPacket *m_frame;

private:
    // raw buffer
    int m_frameSize = 0;
    bool m_findFrameHead = false;
    bool m_findFrameEnd = false;
    int m_frameBufferMaxLen = 0;
    char *m_frameBuffer = 0;
    int m_frameBufferPosStart = 0;
    int m_frameBufferPosEnd = 0;

public:
    AlgoImageParser()
    {
        m_frameBufferMaxLen = 1920 * 1080 * 3 * 2;
        m_frameBuffer = new char[m_frameBufferMaxLen];
        m_frameBufferPosStart = 0;
        m_frameBufferPosEnd = 0;

        m_frame = new AlgoCameraDataPacket();
        m_frame->pbuf = new char[1920 * 1080 * 3];
        m_frame->pbufSize = 0;
    }

    ~AlgoImageParser()
    {
        if (m_frameBuffer)
        {
            delete[] m_frameBuffer;
        }

        if (m_frame->pbuf)
        {
            delete[] m_frame->pbuf;
        }
        delete m_frame;
    }

    virtual bool process(char *pData, int len)
    {
        addData(pData, len);

        if (findFrameData())
        {
            bool result = decodeFrameData(m_frameBuffer + m_frameBufferPosStart, m_frameSize, m_frame);

            moveFrameData();

            return result;
        }

        return false;
    }

    virtual void onData(char *pData, int len)
    {
        printf("Data for Cmd: %s, len: %d\n", pData, len);
    }

    virtual void onError(int nError, char *pData, int len)
    {
        printf("ERROR for Cmd: %s, Number: %d\n", pData, nError);
    }

private:
    bool addData(char *pData, int len)
    {

        if (m_frameBufferPosEnd + len <= m_frameBufferMaxLen)
        {
            memcpy(m_frameBuffer + m_frameBufferPosEnd, pData, len);
            m_frameBufferPosEnd += len;
            return true;
        }
        else
        {
            printf("out of memery,reset memery cache\n", len);
            memcpy(m_frameBuffer, pData, len);
            m_frameBufferPosStart = 0;
            m_frameBufferPosEnd = len;
            return false;
        }
    }

    bool findFrameDataHead()
    {
        for (size_t i = 0; i < m_frameBufferPosEnd - 8 - sizeof(int); i++)
        {
            char *pdata = m_frameBuffer + i;
            if (pdata[0] == '$' && pdata[1] == 'A' && pdata[2] == 'L' && pdata[3] == 'G' && pdata[4] == 'O' && pdata[5] == 'I' && pdata[6] == 'M' && pdata[7] == 'G')
            {
                memcpy((char *)&m_frameSize, pdata + i + 8, sizeof(int));
                m_findFrameHead = true;
                m_frameBufferPosStart = i;
                return true;
            }
        }

        return false;
    }

    bool findFrameDataEnd()
    {
        long s = m_frameBufferPosStart + 256;
        for (size_t i = s; i < m_frameBufferPosEnd - 8; i++)
        {
            char *pdata = m_frameBuffer + i;
            if (pdata[0] == '$' && pdata[1] == 'A' && pdata[2] == 'L' && pdata[3] == 'G' && pdata[4] == 'O' && pdata[5] == 'I' && pdata[6] == 'M' && pdata[7] == 'G')
            {
                //char imgType = pdata[16];
                m_frameSize = i - m_frameBufferPosStart;
                m_findFrameEnd = true;
                return true;
            }
        }

        return false;
    }

    bool findFrameData()
    {
        if (!m_findFrameHead)
        {
            findFrameDataHead();
        }

        if (m_findFrameHead)
        {
            findFrameDataEnd();
        }

        if (m_findFrameHead && m_findFrameEnd)
        {
            return true;
        }

        return false;
    }

    bool decodeFrameData(char *frameData, int frameSize, AlgoCameraDataPacket *image)
    {
        // 解码 JPEG 图像为 Mat 对象
        long headerSize = 256;
        long structSize = sizeof(AlgoCameraDataPacket); // 128 bytes < 256-12
        long dataFrameSize = 0l;
        long jpegSize = frameSize - headerSize;
        memcpy((void *)&dataFrameSize, (void *)(frameData + 8), sizeof(int));
        char *temp = image->pbuf;
        memcpy((void *)image, (void *)(frameData + 12), structSize);
        image->pbuf = temp;
        memcpy(image->pbuf, frameData + headerSize, image->pbufSize);
        return true;
    }

    bool moveFrameData()
    {
        int end = m_frameBufferPosStart + m_frameSize;
        int len = m_frameBufferPosEnd - end;
        memcpy(m_frameBuffer, m_frameBuffer + end, len);
        m_frameBufferPosStart = 0;
        m_frameBufferPosEnd = len;
        m_findFrameHead = false;
        m_findFrameEnd = false;
        m_frameSize = 0;
        return true;
    }
};

#endif //!_AlgoImageParser_H_