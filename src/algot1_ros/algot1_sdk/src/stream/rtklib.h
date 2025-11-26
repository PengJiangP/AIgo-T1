/**
 * @file: RTKLIB.h
 * @brief: This header file contains the declarations and definitions of the RTKLIB library.
 * @version: 2.4.2p13
 * @date: Oct.20,2023
 * @remark: The RTKLIB (Real-Time Kinematic LIBrary) is an open-source library for GNSS positioning. It provides various functions and algorithms for processing GNSS data and generating accurate positioning results.
 */

#ifndef RTKLIB_H
#define RTKLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define VER_RTKLIB "2.4.2" /**< Library version */
#define PATCH_LEVEL "p13"  /**< Patch level */
#define COPYRIGHT_RTKLIB "Copyright (C) 2007-2018 by T.Takasu\nAll rights reserved."

#ifdef WIN32
#define thread_t HANDLE                          /**< Thread handle for Windows */
#define lock_t CRITICAL_SECTION                  /**< Lock for Windows */
#define initlock(f) InitializeCriticalSection(f) /**< Initialization of lock for Windows */
#define lock(f) EnterCriticalSection(f)          /**< Locking for Windows */
#define unlock(f) LeaveCriticalSection(f)        /**< Unlocking for Windows */
#define FILEPATHSEP '\\'                         /**< File path separator for Windows */
#else
#define thread_t pthread_t                      /**< Thread handle for Linux and Unix */
#define lock_t pthread_mutex_t                  /**< Lock for Linux and Unix */
#define initlock(f) pthread_mutex_init(f, NULL) /**< Initialization of lock for Linux and Unix */
#define lock(f) pthread_mutex_lock(f)           /**< Locking for Linux and Unix */
#define unlock(f) pthread_mutex_unlock(f)       /**< Unlocking for Linux and Unix */
#define FILEPATHSEP '/'                         /**< File path separator for Linux and Unix */
#endif

#define SOLQ_NONE 0   /**< Solution status: no solution */
#define SOLQ_FIX 1    /**< Solution status: fix */
#define SOLQ_FLOAT 2  /**< Solution status: float */
#define SOLQ_SBAS 3   /**< Solution status: SBAS */
#define SOLQ_DGPS 4   /**< Solution status: DGPS/DGNSS */
#define SOLQ_SINGLE 5 /**< Solution status: single */
#define SOLQ_PPP 6    /**< Solution status: PPP */
#define SOLQ_DR 7     /**< Solution status: dead reckoning */
#define MAXSOLQ 7     /**< Max number of solution status */

    namespace RTKLIB
    {
        typedef struct
        {                /* Time structure */
            time_t time; /* Time (s) expressed by standard time_t */
            double sec;  /* Fractional part of second (less than 1 s) */
        } gtime_t;
    }

#ifdef __cplusplus
}
#endif

#endif /* RTKLIB_H */
