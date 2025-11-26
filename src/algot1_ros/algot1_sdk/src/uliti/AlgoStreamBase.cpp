#include "AlgoStreamBase.h"
#include "stream/rtkstream.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace RTKLIB;

AlgoStreamBase::AlgoStreamBase()
{
    m_stream_t = new stream_t;
}

AlgoStreamBase::~AlgoStreamBase()
{
    this->close();

    if (m_stream_t)
    {
        delete (stream_t *)m_stream_t;
    }
}

std::string AlgoStreamBase::toPath4Net(std::string ip, int port)
{
    return ip + ":" + std::to_string(port);
}

std::string AlgoStreamBase::toPath4Com(std::string name, int bitrate, std::string parity, int bytesize, int stopbits)
{
    return name + ":" + std::to_string(bitrate) + ":" + std::to_string(bytesize) + ":" + std::to_string(stopbits) + ":" + parity; // ttyS0:460800:8:N:1:none
}

int AlgoStreamBase::netName2netType(std::string netName)
{
    int netType = 0;

    if (netName == "TCPSVR")
    {
        netType = STR_TCPSVR;
    }
    else if (netName == "TCPCLI")
    {
        netType = STR_TCPCLI;
    }
    else if (netName == "UDPSVR")
    {
        netType = STR_UDPSVR;
    }
    else if (netName == "UDPCLI")
    {
        netType = STR_UDPCLI;
    }
    else if (netName == "NTRIPSVR")
    {
        netType = STR_NTRIPSVR;
    }
    else if (netName == "NTRIPCLI")
    {
        netType = STR_NTRIPCLI;
    }
    else if (netName == "CAN")
    {
        netType = STR_CANFD;
    }
    else if (netName == "SERIAL")
    {
        netType = STR_SERIAL;
    }
    else
    {
        netType = STR_NONE;
    }

    return netType;
}

std::string AlgoStreamBase::netType2netName(int nettype)
{
    std::string netName = "";
    switch (nettype)
    {
    case STR_TCPSVR:
        netName = "TCPSVR";
        break;
    case STR_TCPCLI:
        netName = "TCPCLI";
        break;
    case STR_UDPSVR:
        netName = "UDPSVR";
        break;
    case STR_UDPCLI:
        netName = "UDPCLI";
        break;
    case STR_NTRIPSVR:
        netName = "NTRIPSVR";
        break;
    case STR_NTRIPCLI:
        netName = "NTRIPCLI";
        break;
    case STR_CANFD:
        netName = "CAN";
        break;
    case STR_SERIAL:
        netName = "SERIAL";
        break;
    default:
        netName = "";
        break;
    }

    return netName;
}

bool AlgoStreamBase::init(std::string name, int nettype, std::string path,std::string startCmd)
{
    m_startCmdLen = startCmd.length();
    memcpy(m_startCmd,startCmd.c_str(),m_startCmdLen);

    m_path = path;
    m_nettype = nettype;

    stream_t *stream = (stream_t *)m_stream_t;
    ::strinit(stream);

    // stream->streamOptions.toinact = 10000;   /*inactive timeout (ms)*/
    // stream->streamOptions.ticonnect = 10000; /*interval to reconnect (ms)*/
    // stream->streamOptions.tirate = 1000;     /*averaging time of data rate (ms)*/
    // stream->streamOptions.buffsize = 8192;   /*receive/send buffer size (bytes);*/
    // stream->streamOptions.fswapmargin = 30;  /*file swap margin(s)*/
    // stream->streamOptions.mode = STR_MODE_RW;
    // stream->streamOptions.type = nettype;
    // memcpy(stream->streamOptions.path, path.c_str(), path.length());

    return true;
}

std::string AlgoStreamBase::getinfo()
{
    return AlgoStreamBase::netType2netName(m_nettype) + " " + m_path;
}

bool AlgoStreamBase::open()
{
    stream_t *stream = (stream_t *)m_stream_t;
    return ::stropen(stream, m_nettype, STR_MODE_RW, m_path.c_str());
}

int AlgoStreamBase::write(std::string data)
{
    stream_t *stream = (stream_t *)m_stream_t;
    unsigned char *buffer = (unsigned char *)data.c_str();
    int len = data.length();
    return ::strwrite(stream, buffer, len);
}

int AlgoStreamBase::write(unsigned char *buffer, int len)
{
    stream_t *stream = (stream_t *)m_stream_t;
    return ::strwrite(stream, buffer, len);
}

std::string AlgoStreamBase::read()
{
    stream_t *stream = (stream_t *)m_stream_t;
    int maxlen = ALGO_DAT_MAX_LEN;
    unsigned char buffer[ALGO_DAT_MAX_LEN];
    int res = ::strread(stream, buffer, maxlen);
    std::string result((char *)buffer);
    return result;
}

int AlgoStreamBase::read(unsigned char *buffer, int maxlen)
{
    stream_t *stream = (stream_t *)m_stream_t;
    return ::strread(stream, buffer, maxlen);
}

void AlgoStreamBase::close()
{
    stream_t *stream = (stream_t *)m_stream_t;
    ::strclose(stream);
}