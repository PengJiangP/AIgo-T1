/**
 * @file AlgoLogger.h
 * @brief This header file contains the definition of log macro.
 *
 * This file defines different macros for logging messages at different log levels.
 * The logging functionality can be controlled by setting the log level appropriately.
 *
 * @date Oct. 20, 2023
 * @version 1.0
 * @author Jack
 *
 * @note This file is owned by Algo1010 Technology Co., Ltd. All rights reserved.
 */

#ifndef _ALGO_LOGGER_H_
#define _ALGO_LOGGER_H_

namespace Algo1010
{

    /**
     * @def __DEBUG
     * @brief Log module switch.
     *
     * Define this macro to enable log output. Comment it out to disable log output.
     */
#define __DEBUG

#ifdef __DEBUG
#define DEBUG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

    /**
     * @brief Log level enumeration.
     *
     * This enumeration defines different log levels.
     * LOG_LEVEL_OFF: No log will be output.
     * LOG_LEVEL_FATAL: Only fatal error log will be output.
     * LOG_LEVEL_ERR: Error and fatal error logs will be output.
     * LOG_LEVEL_WARN: Warning, error, and fatal error logs will be output.
     * LOG_LEVEL_INFO: Info, warning, error, and fatal error logs will be output.
     * LOG_LEVEL_ALL: All log levels will be output.
     */
    enum LOG_LEVEL
    {
        LOG_LEVEL_OFF = 0,
        LOG_LEVEL_FATAL,
        LOG_LEVEL_ERR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_ALL
    };

    /**
     * @def LogLevel
     * @brief Current log level.
     *
     * You can modify this variable to change the log level.
     * By default, the log level is set to LOG_LEVEL_INFO.
     */
#define LogLevel LOG_LEVEL_INFO

    /**
     * @def ALOGF
     * @brief Macro to log fatal errors.
     *
     * Use this macro to log fatal errors. The log will only be output
     * if the current log level is greater than or equal to LOG_LEVEL_FATAL.
     * The log message will contain the function name, file name, and line number.
     *
     * @param format The log message format.
     * @param ... The log message arguments.
     */
#define ALOGF(format, ...)                                              \
    do                                                                  \
    {                                                                   \
        if (LogLevel >= LOG_LEVEL_FATAL)                                \
            DEBUG("\n->FATAL @ FUNC:%s FILE:%s LINE:%d \n" format "\n", \
                  __func__, __FILE__, __LINE__, ##__VA_ARGS__);         \
    } while (0)

    /**
     * @def ALOGE
     * @brief Macro to log errors.
     *
     * Use this macro to log errors. The log will only be output
     * if the current log level is greater than or equal to LOG_LEVEL_ERR.
     * The log message will contain the function name, file name, and line number.
     *
     * @param format The log message format.
     * @param ... The log message arguments.
     */
#define ALOGE(format, ...)                                              \
    do                                                                  \
    {                                                                   \
        if (LogLevel >= LOG_LEVEL_ERR)                                  \
            DEBUG("\n->ERR   @ FUNC:%s FILE:%s LINE:%d \n" format "\n", \
                  __func__, __FILE__, __LINE__, ##__VA_ARGS__);         \
    } while (0)

    /**
     * @def ALOGW
     * @brief Macro to log warnings.
     *
     * Use this macro to log warnings. The log will only be output
     * if the current log level is greater than or equal to LOG_LEVEL_WARN.
     * The log message will contain the function name.
     *
     * @param format The log message format.
     * @param ... The log message arguments.
     */
#define ALOGW(format, ...)                                                        \
    do                                                                            \
    {                                                                             \
        if (LogLevel >= LOG_LEVEL_WARN)                                           \
            DEBUG("\n->WARN  @ FUNC:%s \n" format "\n", __func__, ##__VA_ARGS__); \
    } while (0)

    /**
     * @def ALOGI
     * @brief Macro to log info messages.
     *
     * Use this macro to log info messages. The log will only be output
     * if the current log level is greater than or equal to LOG_LEVEL_INFO.
     *
     * @param format The log message format.
     * @param ... The log message arguments.
     */
#define ALOGI(format, ...)                                    \
    do                                                        \
    {                                                         \
        if (LogLevel >= LOG_LEVEL_INFO)                       \
            DEBUG("\n->INFO  \n" format "\n", ##__VA_ARGS__); \
    } while (0)

    /**
     * @def ALOGD
     * @brief Macro to log debug messages.
     *
     * Use this macro to log debug messages. The log will only be output
     * if the current log level is greater than or equal to LOG_LEVEL_ALL.
     *
     * @param format The log message format.
     * @param ... The log message arguments.
     */
#define ALOGD(format, ...)                                    \
    do                                                        \
    {                                                         \
        if (LogLevel >= LOG_LEVEL_ALL)                        \
            DEBUG("\n->DEBUG \n" format "\n", ##__VA_ARGS__); \
    } while (0)

} // namespace Algo1010

#endif /* _ALGO_LOGGER_H_ */