#ifndef _ALGO_CRC_CHECK_H_
#define _ALGO_CRC_CHECK_H_

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

namespace Algo1010
{

    class AlgoCrcCheck
    {
    public:
        static ushort getCrc16(unsigned char *pack_buff, int pack_len)
        {
            int len = pack_len;
            ushort crc_result = 0xffff;
            int crc_num = 0;
            int xor_flag = 0;
            for (int i = 0; i < len; i++)
            {
                crc_result ^= pack_buff[i];
                crc_num = (crc_result & 0x0001);
                for (int m = 0; m < 8; m++)
                {
                    if (crc_num == 1)
                        xor_flag = 1;
                    else
                        xor_flag = 0;
                    crc_result >>= 1;
                    if (xor_flag)
                        crc_result ^= 0xa001;
                    crc_num = (crc_result & 0x0001);
                }
            }
            return crc_result;
        }
    };

}

#endif
