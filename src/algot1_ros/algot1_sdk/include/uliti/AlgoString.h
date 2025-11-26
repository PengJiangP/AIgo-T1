/**
 * @file: AlgoString.h
 * @brief: This header file contains the definition of the AlgoString class.
 * It provides various string manipulation functions.
 * @author: jack <283422622@qq.com>
 * @date: Oct.20,2023
 * @version: 1.0
 * @copyright (c) 2023 by Algo1010 Technologny Co.,Ltd.
 * All rights reserved.
 */

#ifndef _ALGO_STRING_H_
#define _ALGO_STRING_H_

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

namespace Algo1010
{

    /**
     * @class AlgoString
     * @brief The AlgoString class provides various string manipulation functions.
     */
    class AlgoString
    {
    public:
        /**
         * @brief Trim whitespace characters from the beginning of a string.
         * @param s The input string to be trimmed (modified in-place).
         */
        static inline void string_ltrim(std::string &s)
        {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
                                            { return !std::isspace(ch); }));
        }

        /**
         * @brief Trim whitespace characters from the end of a string.
         * @param s The input string to be trimmed (modified in-place).
         */
        static inline void string_rtrim(std::string &s)
        {
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
                                 { return !std::isspace(ch); })
                        .base(),
                    s.end());
        }

        /**
         * @brief Trim whitespace characters from both ends of a string.
         * @param s The input string to be trimmed (modified in-place).
         */
        static inline void string_trim(std::string &s)
        {
            string_ltrim(s);
            string_rtrim(s);
        }

        /**
         * @brief Split a string into substrings using a delimiter.
         * @param str The input string to be split.
         * @param delim The delimiter used for splitting the string.
         * @return A vector of substrings obtained after splitting.
         */
        static std::vector<std::string> string_split(const std::string &str, char delim)
        {
            std::size_t previous = 0;
            std::size_t current = str.find(delim);
            std::vector<std::string> elems;
            while (current != std::string::npos)
            {
                if (current >= previous)
                {
                    elems.push_back(str.substr(previous, current - previous));
                }
                previous = current + 1;
                current = str.find(delim, previous);
            }
            if (previous != str.size())
            {
                elems.push_back(str.substr(previous));
            }
            return elems;
        }

        static std::string string_from_char_hex(unsigned char *data, int len)
        {
            std::stringstream ss;
            for (int i = 0; i < len; i++)
            {
                ss << ""<<std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(*(data + i)) << " ";
            }
            std::string hex_str = ss.str();
            hex_str.pop_back();
            return hex_str;
        }
    };

}

#endif //!_ALGO_STRING_H_