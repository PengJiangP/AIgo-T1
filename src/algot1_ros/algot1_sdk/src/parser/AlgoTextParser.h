#ifndef _AlgoTextParser_H_
#define _AlgoTextParser_H_

#include "AlgoSDK.h"
#include "uliti/AlgoParserBase.h"
#include "uliti/AlgoCmdLine.h"
#include "uliti/AlgoString.h"
#include "uliti/AlgoUtc2Gpst.h"

#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define MAX_TEXT_BUFFER_LEN 20480 // 20k bytes, about 2 second for ALGOIMU and INSPVAXA
#define MAX_TEXT_VECTOR_LEN 200   // 200 packats, about 2 second for ALGOIMU and INSPVAXA

namespace Algo1010
{

    enum AlgoTextDataType
    {
        ALGO_NONE = 0,
        ALGO_ALGOIMU,    // header = $ALGOIMU;$1010IMU
        ALGO_INSPVAXA,   // header = $INSPVAXA
        ALGO_NMEA_GPGGA, // header = $GPGGA
        ALGO_NMEA_GPRMC, // header = $GPRMC
        ALGO_ALGOVINS    // header = $VINSPOS;???
    };

    class AlgoTextParser : public AlgoParserBase
    {

    private:
        double D2R = (3.14159265358979323846 / 180.0);
        std::string m_data = ""; // raw text data
        int m_index = 0;
        AlgoTextDataType m_dataType = AlgoTextDataType::ALGO_NONE; // current parser datatype

        std::vector<AlgoDataPacketPtr> m_packetsALGOIMU;
        std::vector<AlgoDataPacketPtr> m_packets1010IMU;
        std::vector<AlgoDataPacketPtr> m_packetsINSPVAXA;
        std::vector<AlgoDataPacketPtr> m_packetsGPGGA;
        std::vector<AlgoDataPacketPtr> m_packetsGPRMC;
        std::vector<AlgoDataPacketPtr> m_packetsVINSPOS;

        std::map<AlgoTextDataType, std::vector<AlgoDataPacketPtr>> m_packetsMap;
        std::map<AlgoTextDataType, std::string> m_headerMap;

    public:
        AlgoTextParser(AlgoTextDataType dataType)
        {
            m_dataType = dataType;

            m_packetsMap[ALGO_ALGOIMU] = m_packetsALGOIMU;
            m_packetsMap[ALGO_INSPVAXA] = m_packetsINSPVAXA;
            m_packetsMap[ALGO_NMEA_GPGGA] = m_packetsGPGGA;
            m_packetsMap[ALGO_NMEA_GPRMC] = m_packetsGPRMC;
            m_packetsMap[ALGO_ALGOVINS] = m_packetsVINSPOS;

            m_headerMap[ALGO_ALGOIMU] = "$ALGOIMU;$1010IMU";
            m_headerMap[ALGO_INSPVAXA] = "#INSPVAXA;$ALGOGNSSATT";
            m_headerMap[ALGO_NMEA_GPGGA] = "$GPGGA";
            m_headerMap[ALGO_NMEA_GPRMC] = "$GPRMC";
            m_headerMap[ALGO_ALGOVINS] = "$VINSPOS";
        }

        ~AlgoTextParser()
        {
        }

    public:
        bool process(char *pData, int len)
        {
            addData(pData, len);

            bool result = false;
            std::vector<std::string> headers = AlgoString::string_split(m_headerMap[m_dataType], ';');
            for (size_t i = 0; i < headers.size(); i++)
            {
                std::string frameText = findFrameData(headers[i]);
                while (!frameText.empty())
                {
                    result = decodeFrameData(frameText);
                    frameText = findFrameData(headers[i]);
                }
            }
            return result;
        }

        std::vector<AlgoDataPacketPtr> popPacket()
        {
            std::vector<AlgoDataPacketPtr> frames;
            frames.assign(m_packetsMap[m_dataType].begin(), m_packetsMap[m_dataType].end());
            m_packetsMap[m_dataType].clear();
            return frames;
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
        double dm2dd(std::string ddmm0mmmm)
        {
            double dd0mmmmmm = std::stod(ddmm0mmmm) / 100.0;
            double dd = (int)dd0mmmmmm;
            double dd0dddddd = dd + (dd0mmmmmm - dd) * 100 / 60.0;
            return dd0dddddd; //*DEG2RAD
        }

        bool checkNmea(std::string frameText)
        {
            return true;
        }

        bool writeFile(std::string pathName)
        {
            // pathName = "/home/AlgoUserData/imu.txt";
            std::ofstream file(pathName, std::ios::app);
            if (file.is_open())
            {
                file.write(m_data.c_str(), m_data.size());
            }
            file.close();
            m_data = "";
            return true;
        }

        bool addData(char *pData, int len)
        {
            m_data += std::string(pData);

            if (m_data.size() > MAX_TEXT_BUFFER_LEN)
            {
                m_data.erase(0, MAX_TEXT_BUFFER_LEN / 2);
            }

            if (m_packetsMap[m_dataType].size() > MAX_TEXT_VECTOR_LEN)
            {
                m_packetsMap[m_dataType].clear();
            }

            return true;
        }

        std::string findFrameData(std::string head)
        {
            if (head.empty())
                return "";

            std::string frameText = "";
            size_t pos1 = m_data.find_first_of(head.c_str(), 0);
            size_t pos2 = m_data.find_first_of("\n", pos1 + 1);
            // if(m_dataType == ALGO_NMEA_GPRMC)
            // printf("findFrameData: pos1=%d,pos2=%d,m_data=%d:%s\n", pos1, pos2, m_data.size(), m_data.c_str());
            if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1)
            {
                frameText = m_data.substr(pos1, pos2 - pos1);
                m_data.erase(0, pos2 + 1);
            }

            return frameText;
        }

        bool decodeFrameData(std::string frameText)
        {
            std::vector<std::string> frameTextSplit;
            frameTextSplit = AlgoString::string_split(frameText, ',');

            if (frameTextSplit.size() == 11 && frameTextSplit[0] == "$ALGOIMU")
            {
                return decodeALGOIMU(frameTextSplit);
            }
            else if (frameTextSplit.size() == 11 && frameTextSplit[0] == "$1010IMU")
            {
                return decode1010IMU(frameTextSplit);
            }
            else if (frameTextSplit.size() == 32 && frameTextSplit[0] == "#INSPVAXA")
            {
                return decodeINSPVAXA(frameTextSplit);
            }
            else if (frameTextSplit[0] == "$ALGOGNSSATT") // frameTextSplit.size() == 17 &&
            {
                return decodeALGOGNSSATT(frameTextSplit);
            }
            else if (frameTextSplit.size() == 15 && frameTextSplit[0] == "$GPGGA")
            {
                //printf("******decodeGPGGA%s\n",frameText.c_str());
                return decodeGPGGA(frameTextSplit);
            }
            else if (frameTextSplit[0] == "$GPRMC") // frameTextSplit.size() == 13 &&
            {
                //printf("******decodeGPRMC:%s \n", frameText.c_str());
                return decodeGPRMC(frameTextSplit);
            }
            else if (frameTextSplit.size() == 25 && frameTextSplit[0] == "$VINSPOS")
            {
                return decodeVINSPOS(frameTextSplit);
            }
            else
            {
                // unknown data format
                printf("decodeFrameData, unknown data format:(%d bytes, %d segmets)%s\n m_data=%d\n",
                       frameText.size(), frameTextSplit.size(), frameText.c_str(), m_data.size());
            }

            return false;
        }

        //$ALGOIMU, 2292,  180885.274, 20.869, 1.885310,  10.134396, -4.528053, -0.914700, -0.010477,  0.426448,00101*4B
        bool decodeALGOIMU(std::vector<std::string> frameTextSplit)
        {
            AlgoImuDataPacketPtr packet(new AlgoImuDataPacket());
            packet->index = m_index++;
            // decode $ALGOIMU
            packet->hasPPS = true; // sync with gnss pps
            packet->cur_week = std::stoi(frameTextSplit[1]);
            packet->cur_sec = std::stod(frameTextSplit[2]);
            packet->curtick_ms = std::stod(frameTextSplit[3]) * 1000;

            packet->gyro_x = std::stod(frameTextSplit[4]);
            packet->gyro_y = std::stod(frameTextSplit[5]);
            packet->gyro_z = std::stod(frameTextSplit[6]);
            packet->accel_x = std::stod(frameTextSplit[7]);
            packet->accel_y = std::stod(frameTextSplit[8]);
            packet->accel_z = std::stod(frameTextSplit[9]);
            m_packetsMap[ALGO_ALGOIMU].push_back(packet);

            return true;
        }

        //$1010IMU, 0,  20.869, 20.869, -0.000046, -0.062500,  0.000092, -0.996065,  0.020899,  0.051880,00300*4B
        bool decode1010IMU(std::vector<std::string> frameTextSplit)
        {
            AlgoImuDataPacketPtr packet(new AlgoImuDataPacket());
            packet->index = m_index++;
            // decode $ALGOIMU
            packet->hasPPS = false; // not sync with gnss pps
            packet->cur_week = std::stoi(frameTextSplit[1]);
            packet->cur_sec = std::stod(frameTextSplit[2]);
            packet->curtick_ms = std::stod(frameTextSplit[3]) * 1000;

            packet->gyro_x = std::stod(frameTextSplit[4]);
            packet->gyro_y = std::stod(frameTextSplit[5]);
            packet->gyro_z = std::stod(frameTextSplit[6]);
            packet->accel_x = std::stod(frameTextSplit[7]);
            packet->accel_y = std::stod(frameTextSplit[8]);
            packet->accel_z = std::stod(frameTextSplit[9]);
            m_packetsMap[ALGO_ALGOIMU].push_back(packet);

            return true;
        }

        //$ALGOGNSSATT
        bool decodeALGOGNSSATT(std::vector<std::string> frameTextSplit)
        {
            AlgoINSPVAXADataPacketPtr packet(new AlgoINSPVAXADataPacket());
            packet->index = m_index++;
            packet->hasPPS = true;
            packet->cur_week = std::stoi(frameTextSplit[1]);
            packet->cur_sec = std::stod(frameTextSplit[2]);
            // packet->posLat = std::stod(frameTextSplit[11]) * D2R;
            // packet->posLon = std::stod(frameTextSplit[12]) * D2R;
            // packet->posHgt = std::stod(frameTextSplit[13]);
            // packet->velN = std::stod(frameTextSplit[15]);
            // packet->velE = std::stod(frameTextSplit[16]);
            // packet->velU = std::stod(frameTextSplit[17]);
            packet->yaw = std::stod(frameTextSplit[3]);
            packet->roll = std::stod(frameTextSplit[4]);
            packet->pitch = std::stod(frameTextSplit[5]);

            packet->insStatus = 0;
            std::vector<std::string> insStatusText;
            m_packetsMap[ALGO_INSPVAXA].push_back(packet);
        }

        // #INSPVAXA,sdcard_5,53,0.0,FINESTEERING,2292,180635.903,00000000,0000,1;INS_SOLUTION_GOOD,INS_RTKFIXED,30.83542349446,121.34663881156,201.7819,10.9075,-0.2293,-0.2793,0.4898,-35.892507277,8.467863288,7.283741505,0.0286,0.0082,0.0235,0.0006,0.0008,0.0014,0.5689,0.4698,1.3437,00000000,0*14D20C51
        bool decodeINSPVAXA(std::vector<std::string> frameTextSplit)
        {
            AlgoINSPVAXADataPacketPtr packet(new AlgoINSPVAXADataPacket());
            packet->index = m_index++;
            packet->hasPPS = false;
            packet->cur_week = std::stoi(frameTextSplit[5]);
            packet->cur_sec = std::stod(frameTextSplit[6]);
            packet->posLat = std::stod(frameTextSplit[11]) * D2R;
            packet->posLon = std::stod(frameTextSplit[12]) * D2R;
            packet->posHgt = std::stod(frameTextSplit[13]);

            packet->velN = std::stod(frameTextSplit[15]);
            packet->velE = std::stod(frameTextSplit[16]);
            packet->velU = std::stod(frameTextSplit[17]);

            packet->roll = std::stod(frameTextSplit[18]);
            packet->pitch = std::stod(frameTextSplit[19]);
            packet->yaw = std::stod(frameTextSplit[20]);

            packet->sigLat = std::stod(frameTextSplit[21]);
            packet->sigLon = std::stod(frameTextSplit[22]);
            packet->sigHgt = std::stod(frameTextSplit[23]);
            packet->sigVelN = std::stod(frameTextSplit[24]);
            packet->sigVelE = std::stod(frameTextSplit[25]);
            packet->sigVelU = std::stod(frameTextSplit[26]);
            packet->sigYaw = std::stod(frameTextSplit[27]);
            packet->sigRoll = std::stod(frameTextSplit[28]);
            packet->sigPitch = std::stod(frameTextSplit[29]);

            // 0 INS_INACTIVE 对准未激活
            // 1 INS_ALIGNING 正在进行粗对准
            // 2 INS_HIGH_VARIANCE 较高协方差，姿态估计未收敛
            // 3 INS_SOLUTION_GOOD 对准完成结果较好
            // 6 INS_SOLUTION_FREE 卫星结果较差不可用
            // 7 INS_ALIGNMENT_COMPLETE 粗对准完成
            // 8 DETERMINING_ORIENTATION 正在确定 IMU 轴与重力对齐
            // 9 WAITING_INITIALPOS 等待位置解
            // 10 WAITING_AZIMUTH 等待航向角
            // 11 INITIALIZING_BIASES 在静态数据前 10 秒内估计初始偏差
            // 12 MOTION_DETECT 尚未完全对准，但已检测到运动
            packet->insStatus = 0;
            std::vector<std::string> insStatusText;
            insStatusText = AlgoString::string_split(frameTextSplit[9], ';');
            if (insStatusText.size() == 2)
            {
                if (insStatusText[1] == "INS_INACTIVE")
                    packet->insStatus = 0;
                else if (insStatusText[1] == "INS_ALIGNING")
                    packet->insStatus = 1;
                else if (insStatusText[1] == "INS_HIGH_VARIANCE")
                    packet->insStatus = 2;
                else if (insStatusText[1] == "INS_SOLUTION_GOOD")
                    packet->insStatus = 3;
                else if (insStatusText[1] == "INS_SOLUTION_FREE")
                    packet->insStatus = 6;
                else if (insStatusText[1] == "INS_ALIGNMENT_COMPLETE")
                    packet->insStatus = 7;
                else if (insStatusText[1] == "DETERMINING_ORIENTATION")
                    packet->insStatus = 8;
                else if (insStatusText[1] == "WAITING_INITIALPOS")
                    packet->insStatus = 9;
                else if (insStatusText[1] == "WAITING_AZIMUTH")
                    packet->insStatus = 10;
                else if (insStatusText[1] == "INITIALIZING_BIASES")
                    packet->insStatus = 11;
                else if (insStatusText[1] == "MOTION_DETECT")
                    packet->insStatus = 12;
                else
                    packet->insStatus = 0;
            }

            if( packet->insStatus > 1)
               packet->hasPPS = true;

            //INS_PSRSP，单点；INS_PSRDIFF，RTD；INS_RTKFLOAT，浮点；INS_RTKFIXED，固定；NONE，GNSS定位失败
            packet->posType = 0;
            if(frameTextSplit[10] == "INS_PSRSP")
                packet->posType = 1;
            else if(frameTextSplit[10] == "INS_PSRDIFF")
                packet->posType = 2;
            else if(frameTextSplit[10] == "INS_RTKFLOAT")
                packet->posType = 3;
            else if(frameTextSplit[10] == "INS_RTKFIXED")
                packet->posType = 4;
            else
                packet->posType = 0;

            m_packetsMap[ALGO_INSPVAXA].push_back(packet);

            return true;
        }

        //$GPGGA,020326.20,3050.1262631,N,12120.7980944,E,4,40,1.0,104.533,M,10.908,M,2.2,0481*42
        bool decodeGPGGA(std::vector<std::string> frameTextSplit)
        {
            AlgoGPGGADataPacketPtr packet(new AlgoGPGGADataPacket());
            packet->index = m_index++;


            std::string hhmmss = frameTextSplit[1] + "0";
            int gps_week = 0;
            double gps_second = 0;
            if (AlgoUtc2Gpst::algo_utc2gpst(nullptr, hhmmss.c_str(), gps_week, gps_second))
            {
                packet->cur_week = gps_week;
                packet->cur_sec = gps_second;
                packet->hasPPS = true;
            }
            else
            {
                packet->hasPPS = false;
            }

            packet->posLat = dm2dd(frameTextSplit[2] + "0"); // ddmm.mmmmmm 去掉后面的（* D2R） 保持度为单位
            packet->posLon = dm2dd(frameTextSplit[4] + "0"); // dddmm.mmmmm 去掉后面的（* D2R） 保持度为单位

            packet->posHgt = std::stod(frameTextSplit[9] + "0");
            packet->posType = std::stoi("0" + frameTextSplit[6]);
            packet->satNum = std::stoi("0" + frameTextSplit[7]);
            // --- 新增下面这一行 (解析 HDOP) ---
            // GPGGA 格式中 HDOP 是第9个元素 (索引8)
            // $GPGGA,time,lat,ns,lon,ew,quality,numsv,HDOP,alt,...
            if(frameTextSplit.size() > 8) {
                packet->hdop = std::stod("0" + frameTextSplit[8]);
            } else {
                packet->hdop = 99.9; // 默认给一个大值表示不可靠
            }
            // -------------------------------
            packet->diffAge = std::stod("0" + frameTextSplit[13]);

            if (frameTextSplit[3] == "W")
            {
                packet->posLon = -packet->posLon;
            }

            if (frameTextSplit[5] == "S")
            {
                packet->posLat = -packet->posLat;
            }

            m_packetsMap[ALGO_NMEA_GPGGA].push_back(packet);
            return true;
        }

        //$GPRMC,020807.40,A,3050.1252431,N,12120.7985627,E,0.08,169.71,121223,0.0,E,D*3C
        //$GPRMC,150449.00,A,3109.9959345,N,12117.3417355,E,0.02, 74.44,280824,0.0,E,A*0E
        bool decodeGPRMC(std::vector<std::string> frameTextSplit)
        {
            AlgoGPRMCDataPacketPtr packet(new AlgoGPRMCDataPacket());
            packet->index = m_index++;

            std::string hhmmss = frameTextSplit[1] + "0";
            std::string ddmmyy = frameTextSplit[9];
            int gps_week = 0;
            double gps_second = 0;
            if (!ddmmyy.empty() && AlgoUtc2Gpst::algo_utc2gpst(ddmmyy.c_str(), hhmmss.c_str(), gps_week, gps_second))
            {
                packet->cur_week = gps_week;
                packet->cur_sec = gps_second;
                packet->hasPPS = true;
            }
            else
            {
                packet->hasPPS = false;
            }

            if (frameTextSplit[2] == "A")
                packet->posType = 1;
            else // V
                packet->posType = 0;

            packet->posLat = dm2dd(frameTextSplit[3] + "0") * D2R; // ddmm.mmmmmm
            packet->posLon = dm2dd(frameTextSplit[5] + "0") * D2R; // dddmm.mmmmm
            packet->posHgt = 0.0;

            m_packetsMap[ALGO_NMEA_GPRMC].push_back(packet);
            return true;
        }

        //$VINSPOS,139638.042,-0.711044,0.703133,-0.004060,0.002102,-0.015,-0.069,-0.003,0.006,-0.029,-0.003,0.002045,0.002049,0.020007,0.050,0.050,0.050,0.002,0.003,0.002,-0.0307991,1,FF
        bool decodeVINSPOS(std::vector<std::string> frameTextSplit)
        {
            AlgoVINSPOSDataPacketPtr packet(new AlgoVINSPOSDataPacket());
            packet->index = m_index++;
            packet->hasPPS = true;
            packet->cur_week = std::stoi(frameTextSplit[1]);
            packet->cur_sec = std::stod(frameTextSplit[2]);
            packet->curtick_ms = std::stod(frameTextSplit[3]) * 1000.0;

            packet->Q[0] = std::stod(frameTextSplit[4]);
            packet->Q[1] = std::stod(frameTextSplit[5]);
            packet->Q[2] = std::stod(frameTextSplit[6]);
            packet->Q[3] = std::stod(frameTextSplit[7]);

            packet->P[0] = std::stod(frameTextSplit[8]);
            packet->P[1] = std::stod(frameTextSplit[9]);
            packet->P[2] = std::stod(frameTextSplit[10]);

            packet->V[0] = std::stod(frameTextSplit[11]);
            packet->V[1] = std::stod(frameTextSplit[12]);
            packet->V[2] = std::stod(frameTextSplit[13]);

            packet->stdQ[0] = std::stod(frameTextSplit[14]);
            packet->stdQ[1] = std::stod(frameTextSplit[15]);
            packet->stdQ[2] = std::stod(frameTextSplit[16]);

            packet->stdP[0] = std::stod(frameTextSplit[17]);
            packet->stdP[1] = std::stod(frameTextSplit[18]);
            packet->stdP[2] = std::stod(frameTextSplit[19]);

            packet->stdV[0] = std::stod(frameTextSplit[20]);
            packet->stdV[1] = std::stod(frameTextSplit[21]);
            packet->stdV[2] = std::stod(frameTextSplit[22]);

            packet->td = std::stod(frameTextSplit[23]);

            m_packetsMap[ALGO_ALGOVINS].push_back(packet);
            return true;
        }
    };

}

#endif //!_AlgoTextParser_H_
