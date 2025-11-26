/**
 * @file AlgoSystemTime.h
 * @brief This header file contains the definition of AlgoSystemTime class.
 *
 * @details The AlgoSystemTime class provides functions to handle system time operations.
 * It includes functions to get the current time, format the date and time, etc.
 *
 * @author Jack
 * @date Oct.20,2023
 * @version 1.0
 *
 * @note This file is created by Algo1010 Technology Co.,Ltd. All rights reserved.
 */

#ifndef _ALGOSYSTEMTIME_H_
#define _ALGOSYSTEMTIME_H_

#include <iostream>
#include <ctime>
#include <string>

#include "AlgoData.h"

namespace Algo1010
{

    /**
     * @class AlgoSystemTime
     * @brief A class for handling system time operations.
     */
    class AlgoSystemTime
    {
    public:
        /**
         * @brief Get the current system time.
         * @return The current system time in seconds since January 1, 1970.
         */
        static double getCurrentTime()
        {
            struct timespec currentTime;
            clock_gettime(CLOCK_REALTIME, &currentTime);
            long int milliseconds = currentTime.tv_nsec / 1000000;
            double td = currentTime.tv_sec + milliseconds / 1000.0;
            return td;
        }

        /**
         * @brief Get the formatted date from a given time.
         * @param time The input time in seconds since January 1, 1970.
         * @param split The delimiter to separate year, month, and day. Default value is an empty string.
         * @return The formatted date string in the format "YYYYMMDD".
         */
        static std::string getFormattedDate(double time, std::string split = "")
        {
            struct timespec currentTime;
            currentTime.tv_sec = (long)time;
            currentTime.tv_nsec = (time - (long)time) * 1000000 * 1000;

            time_t rawTime = currentTime.tv_sec;
            struct tm *timeInfo = localtime(&rawTime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y%m%d", timeInfo);

            return buffer;
        }

        /**
         * @brief Get the formatted time from a given time.
         * @param time The input time in seconds since January 1, 1970.
         * @param split The delimiter to separate hours, minutes, and seconds. Default value is an empty string.
         * @return The formatted time string in the format "HHMMSS".
         */
        static std::string getFormattedTime(double time, std::string split = "")
        {
            struct timespec currentTime;
            currentTime.tv_sec = (long)time;
            currentTime.tv_nsec = (time - (long)time) * 1000000 * 1000;

            time_t rawTime = currentTime.tv_sec;
            struct tm *timeInfo = localtime(&rawTime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%H%M%S", timeInfo);

            return buffer;
        }
    };

}

#endif //!_ALGOSYSTEMTIME_H_