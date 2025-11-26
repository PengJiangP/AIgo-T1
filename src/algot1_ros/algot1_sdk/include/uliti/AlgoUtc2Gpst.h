#ifndef _ALGO_UTC_GPST_H_
#define _ALGO_UTC_GPST_H_

#include <stdio.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <mutex>

namespace Algo1010
{

    static std::string m_ddmmyy = "";
    static std::mutex m_lock_ddmmyy;


    class AlgoUtc2Gpst
    {
    public:
    public:
        AlgoUtc2Gpst(/* args */)
        {
        }
        ~AlgoUtc2Gpst()
        {
        }

        // 手动实现timegm函数（处理UTC时间）
        static time_t algo_timegm(struct tm *tm)
        {
            time_t t = mktime(tm);
            return t; // - (time_t)(mktime(localtime(&t)) - mktime(gmtime(&t)));
        }

        static bool algo_utc2gpst(const char *date_str, const char *time_str, int &gps_week, double &gps_seconds)
        {
            // char date_str[] = "010124";  // 输入日期：DDMMYY格式（示例：01 Jan 2024）
            // char time_str[] = "123456.78"; // 输入时间：HHMMSS.SS格式（示例：12:34:56.78）

            // 解析日期：DDMMYY
            int day, month, year;
            if (date_str == nullptr)
            {
                std::lock_guard<std::mutex> lock(m_lock_ddmmyy);

                if (m_ddmmyy.empty())   
                return false;
                sscanf(m_ddmmyy.c_str(), "%2d%2d%2d", &day, &month, &year);
            }
            else
            {
                std::lock_guard<std::mutex> lock(m_lock_ddmmyy);
                std::string date(date_str);
                if (date != m_ddmmyy)
                {
                    m_ddmmyy = date;
                }
                sscanf(date_str, "%2d%2d%2d", &day, &month, &year);
            }
            
            // 处理两位年份（假设80-99表示1980-1999，00-79表示2000-2079）
            year += (year >= 80) ? 1900 : 2000;

            // 解析时间：HHMMSS.SS
            int hh, mm, ss_int;
            int ss_frac;
            sscanf(time_str, "%2d%2d%2d.%3d", &hh, &mm, &ss_int, &ss_frac);

            // 构造tm结构体（UTC时间）
            struct tm utc_tm = {0};
            utc_tm.tm_year = year - 1900;
            utc_tm.tm_mon = month - 1;
            utc_tm.tm_mday = day;
            utc_tm.tm_hour = hh;
            utc_tm.tm_min = mm;
            utc_tm.tm_sec = ss_int;
            utc_tm.tm_isdst = -1; // 不处理夏令时

            // 转换为time_t（UTC秒数）
            time_t utc_time = algo_timegm(&utc_tm);
            if (utc_time == -1)
            {
                printf("Invalid date/time input.\n");
                return false;
            }

            // GPS起始时间：1980年1月6日 00:00:00 UTC
            struct tm gps_epoch_tm = {0};
            gps_epoch_tm.tm_year = 80; // 1980 - 1900
            gps_epoch_tm.tm_mon = 0;   // January
            gps_epoch_tm.tm_mday = 6;
            time_t gps_epoch = algo_timegm(&gps_epoch_tm);

            // 计算总秒数差（含闰秒调整和小数秒）
            double total_sec = difftime(utc_time, gps_epoch) + 3600 + 18 + ss_frac / 1000.0; // 闰秒数截至2023年为18, 3600???

            // 计算GPS周和秒
            gps_week = (int)(total_sec / 604800); // 每周秒数 = 7 * 86400
            gps_seconds = fmod(total_sec, 604800);

            // 输出结果
            //printf("(%d-%d-%d   %d:%d:%d.%d  *  GPS Week: %d, GPS Seconds: %.3f)\n", day, month, year, hh, mm, ss_int, ss_frac, gps_week, gps_seconds);

            return true;
        }
    };

}

#endif //_ALGO_UTC_GPST_H_