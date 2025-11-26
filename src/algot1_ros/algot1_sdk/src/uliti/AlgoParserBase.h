/**
 * @file: AlgoParserBase.h
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @brief: This header file contains the definition of AlgoParserBase.
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _AlgoParserBase_H_
#define _AlgoParserBase_H_

#include "AlgoData.h"

#include <string.h>
#include <vector>


#define ALGO_CMD_MAX_LEN 128

namespace Algo1010
{
    class AlgoCmdBinary
    {
    public:
        unsigned char m_cmd[ALGO_CMD_MAX_LEN];
        int m_cmdLen = 0;
    };

    class AlgoCmdString
    {
    public:
        std::string m_cmd = "";

    public:
        AlgoCmdBinary toCmdBinary()
        {
            AlgoCmdBinary cmd;
            cmd.m_cmdLen = m_cmd.length();
            ::memcpy(cmd.m_cmd, m_cmd.c_str(), cmd.m_cmdLen);
            return cmd;
        }
    };

    class AlgoParserBase
    {
    public:
        std::string m_name = "";

    public:
        AlgoParserBase() {}
        ~AlgoParserBase() {}

        virtual bool process(char *pData, int len) = 0;

        virtual std::vector<AlgoDataPacketPtr> popPacket()
        {
            std::vector<AlgoDataPacketPtr> frames;
            return frames;
        }

        virtual void onData(char *pData, int len)
        {
            printf("%s Recv Data: %s, len: %d\n",m_name.c_str(), pData, len);
        }

        virtual void onError(int nError, char *pData, int len)
        {
            printf("%s Recv Error: %s, errno: %d\n",m_name.c_str(), pData, nError);
        }
    };

}

#endif //!_AlgoParserBase_H_