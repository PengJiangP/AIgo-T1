/*******************************************************************************
 *                       RTKLIB Time Helper                                    *
 *******************************************************************************
 * FileName:       time.h
 * Dependencies:   stdio.h, stdlib.h,stdarg.h, string.h,math.h, time.h,ctype.h
 *                 winsock2.h,windows.h(partially on Windows platform),pthread.h(on non-Windows platforms)
 * Header File:    rtklib.h
 * Authors:        RTKLIB developers
 *
 * Description:
 * This is the header file for the RTKLIB time module. It provides various time and string manipulation functions for
 * working with GPS time and epoch time.
 *
 * Classes/Functions:
 *
 * - str2num: Converts a string of characters to a number.
 * - str2time: Converts a string of characters to a time in gtime_t format.
 * - time2str: Converts time in gtime_t format to a string representation.
 * - epoch2time: Converts an epoch time (year, month, day, hour, minute, second) to gtime_t format.
 * - time2epoch: Converts a time in gtime_t format to an epoch time (year, month, day, hour, minute, second).
 * - gpst2time: Converts GPS time (week number and seconds of week) to gtime_t format.
 * - time2gpst: Converts time in gtime_t format to GPS time (week number only).
 * - gst2time: Converts Galileo system time (week number and seconds of week) to gtime_t format.
 * - time2gst: Converts time in gtime_t format to Galileo system time (week number only).
 * - bdt2time: Converts BeiDou time (week number and seconds of week) to gtime_t format.
 * - time2bdt: Converts time in gtime_t format to BeiDou time (week number only).
 * - time_str: Convert a gtime_t time to a formatted string.
 * - timeadd: Adds a given number of seconds to the given gtime_t time.
 * - timediff: Calculates the time difference between two gtime_t times in seconds.
 * - gpst2utc: Converts GPS time to UTC time.
 * - utc2gpst: Converts UTC time to GPS time.
 * - gpst2bdt: Converts GPS time to BeiDou time.
 * - bdt2gpst: Converts BeiDou time to GPS time.
 * - timeget: Gets the current system time in gtime_t format.
 * - timeset: Sets the current system time to a given gtime_t time.
 * - time2doy: Calculates the day of year for a given gtime_t time.
 * - utc2gmst: Converts UTC time to Greenwich Mean Sidereal Time (GMST).
 * - read_leaps: Reads leap second data from a file and stores it in an internal table.
 * - adjgpsweek: Adjusts the GPS week number for a given week number.
 * - tickget: Gets the elapsed time in milliseconds from an arbitrary reference point.
 * - sleepms: Pauses the program execution for a given number of milliseconds.
 * - traceopen: Initializes trace or log output to a specified file.
 * - traceclose: Closes the trace or log file.
 * - tracelevel: Sets the trace level, which determines what kind of messages to print.
 * - trace: Outputs a formatted trace message.
 * - tracet: Outputs a formatted trace message with a time tag.
 * - tracemat: Outputs a formatted trace message for a matrix (2D array) of doubles.
 * - traceb: Outputs a formatted trace message for a buffer (array) of unsigned char.
 * - execcmd: Executes a command in the operating system.
 * - expath: Expands a file path, including wildcards and time tags.
 * - createdir: Creates a directory with the given path.
 * - reppath: Replaces the wildcards ($) in a file path with actual values.
 * - reppaths: Replaces the wildcards ($) in multiple file paths with actual values between start and end time.
 * - uncompress: Uncompresses a compressed file into a new file.
 *
 *******************************************************************************/

#ifndef RTKLIB_TIME_H
#define RTKLIB_TIME_H

/* Includes ------------------------------------------------------------------*/
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

#include "rtklib.h"

/* Definitions ---------------------------------------------------------------*/
#define MAXLEAPS 64 /* Maximum number of leap seconds */

/* Constants -----------------------------------------------------------------*/
#define PI 3.1415926535897932 /* The mathematical constant pi */
#define TRACE                 /* Enable debug trace */

    /* Namespace -----------------------------------------------------------------*/
    namespace RTKLIB
    {

        /* Function Declarations ---------------------------------------------------*/

        /**
         * @brief Converts a string of characters to a number.
         *
         * @param[in] s  The input string of characters.
         * @param[in] i  The starting index of the substring.
         * @param[in] n  The length of the substring.
         *
         * @return The converted number.
         */
        extern double str2num(const char *s, int i, int n);

        /**
         * @brief Converts a string of characters to a time in gtime_t format.
         *
         * @param[in] s  The input string of characters.
         * @param[in] i  The starting index of the substring.
         * @param[in] n  The length of the substring.
         * @param[out] t  The converted time in gtime_t format.
         *
         * @return 1 on success, 0 otherwise.
         */
        extern int str2time(const char *s, int i, int n, gtime_t *t);

        /**
         * @brief Converts time in gtime_t format to a string representation.
         *
         * @param[in] t  The input time in gtime_t format.
         * @param[out] str  The string representation of the time.
         * @param[in] n  The maximum number of characters in the output string.
         *
         * @return none
         */
        extern void time2str(gtime_t t, char *str, int n);

        /**
         * @brief Converts an epoch time (year, month, day, hour, minute, second) to gtime_t format.
         *
         * @param[in] ep The input epoch time as a double array [year, month, day, hour, minute, second].
         *
         * @return The converted time in gtime_t format.
         */
        extern gtime_t epoch2time(const double *ep);

        /**
         * @brief Converts a time in gtime_t format to an epoch time (year, month, day, hour, minute, second).
         *
         * @param[in] t The input time in gtime_t format.
         * @param[out] ep The converted epoch time as a double array [year, month, day, hour, minute, second].
         *
         * @return none
         */
        extern void time2epoch(gtime_t t, double *ep);

        /**
         * @brief Converts GPS time (week number and seconds of week) to gtime_t format.
         *
         * @param[in] week The input GPS week number.
         * @param[in] sec The input GPS seconds of week.
         *
         * @return The converted time in gtime_t format.
         */
        extern gtime_t gpst2time(int week, double sec);

        /**
         * @brief Converts time in gtime_t format to GPS time (week number only).
         *
         * @param[in] t The input time in gtime_t format.
         * @param[out] week The converted GPS week number.
         *
         * @return The GPS seconds of week.
         */
        extern double time2gpst(gtime_t t, int *week);

        /**
         * @brief Converts Galileo system time (week number and seconds of week) to gtime_t format.
         *
         * @param[in] week The input Galileo system week number.
         * @param[in] sec The input Galileo system seconds of week.
         *
         * @return The converted time in gtime_t format.
         */
        extern gtime_t gst2time(int week, double sec);

        /**
         * @brief Converts time in gtime_t format to Galileo system time (week number only).
         *
         * @param[in] t The input time in gtime_t format.
         * @param[out] week The converted Galileo system week number.
         *
         * @return The Galileo system seconds of week.
         */
        extern double time2gst(gtime_t t, int *week);

        /**
         * @brief Converts BeiDou time (week number and seconds of week) to gtime_t format.
         *
         * @param[in] week The input BeiDou week number.
         * @param[in] sec The input BeiDou seconds of week.
         *
         * @return The converted time in gtime_t format.
         */
        extern gtime_t bdt2time(int week, double sec);

        /**
         * @brief Converts time in gtime_t format to BeiDou time (week number only).
         *
         * @param[in] t The input time in gtime_t format.
         * @param[out] week The converted BeiDou week number.
         *
         * @return The BeiDou seconds of week.
         */
        extern double time2bdt(gtime_t t, int *week);

        /**
         * @brief Convert a gtime_t time to a formatted string.
         *
         * @param[in] t The input time in gtime_t format.
         * @param[in] n The maximum number of characters in the output string.
         *
         * @return The formatted string representation of the time.
         */
        extern char *time_str(gtime_t t, int n);

        /**
         * @brief Adds a given number of seconds to the given gtime_t time.
         *
         * @param[in] t The input gtime_t time.
         * @param[in] sec The number of seconds to be added.
         *
         * @return The updated gtime_t time.
         */
        extern gtime_t timeadd(gtime_t t, double sec);

        /**
         * @brief Calculates the time difference between two gtime_t times in seconds.
         *
         * @param[in] t1 The first gtime_t time.
         * @param[in] t2 The second gtime_t time.
         *
         * @return The time difference in seconds.
         */
        extern double timediff(gtime_t t1, gtime_t t2);

        /**
         * @brief Converts GPS time to UTC time.
         *
         * @param[in] t The input GPS time in gtime_t format.
         *
         * @return The corresponding UTC time in gtime_t format.
         */
        extern gtime_t gpst2utc(gtime_t t);

        /**
         * @brief Converts UTC time to GPS time.
         *
         * @param[in] t The input UTS time in gtime_t format.
         *
         * @return The corresponding GPS time in gtime_t format.
         */
        extern gtime_t utc2gpst(gtime_t t);

        /**
         * @brief Converts GPS time to BeiDou time.
         *
         * @param[in] t The input GPS time in gtime_t format.
         *
         * @return The corresponding BeiDou time in gtime_t format.
         */
        extern gtime_t gpst2bdt(gtime_t t);

        /**
         * @brief Converts BeiDou time to GPS time.
         *
         * @param[in] t The input BeiDou time in gtime_t format.
         *
         * @return The corresponding GPS time in gtime_t format.
         */
        extern gtime_t bdt2gpst(gtime_t t);

        /**
         * @brief Gets the current system time in gtime_t format.
         *
         * @return The current system time in gtime_t format.
         */
        extern gtime_t timeget(void);

        /**
         * @brief Sets the current system time to a given gtime_t time.
         *
         * @param[in] t The input gtime_t time.
         *
         * @return none
         */
        extern void timeset(gtime_t t);

        /**
         * @brief Calculates the day of year for a given gtime_t time.
         *
         * @param[in] t The input gtime_t time.
         *
         * @return The day of year.
         */
        extern double time2doy(gtime_t t);

        /**
         * @brief Converts UTC time to Greenwich Mean Sidereal Time (GMST).
         *
         * @param[in] t The input UTC time in gtime_t format.
         * @param[in] ut1_utc The difference between Universal Time Coordinated (UTC) and Universal Time 1 (UT1), in seconds.
         *
         * @return The GMST in radians.
         */
        extern double utc2gmst(gtime_t t, double ut1_utc);

        /**
         * @brief Reads leap second data from a file and stores it in an internal table.
         *
         * @param[in] file The name of the file containing the leap second data.
         *
         * @return The number of leap seconds read on success, -1 otherwise.
         */
        extern int read_leaps(const char *file);

        /**
         * @brief Adjusts the GPS week number for a given week number.
         *
         * @param[in] week The input GPS week number.
         *
         * @return The adjusted GPS week number.
         */
        extern int adjgpsweek(int week);

        /**
         * @brief Gets the elapsed time in milliseconds from an arbitrary reference point.
         *
         * @return The elapsed time in milliseconds.
         */
        extern unsigned int tickget(void);

        /**
         * @brief Pauses the program execution for a given number of milliseconds.
         *
         * @param[in] ms The number of milliseconds to pause.
         *
         * @return none
         */
        extern void sleepms(int ms);

        /**
         * @brief Initializes trace or log output to a specified file.
         *
         * @param[in] file The name of the file to write the log.
         *
         * @return none
         */
        extern void traceopen(const char *file);

        /**
         * @brief Closes the trace or log file.
         *
         * @return none
         */
        extern void traceclose(void);

        /**
         * @brief Sets the trace level, which determines what kind of messages to print.
         *
         * @param[in] level The trace level (0 - 5).
         *
         * @return none
         */
        extern void tracelevel(int level);

        /**
         * @brief Outputs a formatted trace message.
         *
         * @param[in] level The trace level of the message.
         * @param[in] format The format string for the message.
         * @param[in] ... Variable arguments list for the format string.
         *
         * @return none
         */
        extern void trace(int level, const char *format, ...);

        /**
         * @brief Outputs a formatted trace message with a time tag.
         *
         * @param[in] level The trace level of the message.
         * @param[in] format The format string for the message.
         * @param[in] ... Variable arguments list for the format string.
         *
         * @return none
         */
        extern void tracet(int level, const char *format, ...);

        /**
         * @brief Outputs a formatted trace message for a matrix (2D array) of doubles.
         *
         * @param[in] level The trace level of the message.
         * @param[in] A The input 2D array of doubles.
         * @param[in] n The number of rows in the array.
         * @param[in] m The number of columns in the array.
         * @param[in] p The width of each element in the array.
         * @param[in] q The precision of each element in the array.
         *
         * @return none
         */
        extern void tracemat(int level, const double *A, int n, int m, int p, int q);

        /**
         * @brief Outputs a formatted trace message for a buffer (array) of unsigned char.
         *
         * @param[in] level The trace level of the message.
         * @param[in] p The input buffer (array) of unsigned char.
         * @param[in] n The number of elements in the buffer.
         *
         * @return none
         */
        extern void traceb(int level, const unsigned char *p, int n);

        /**
         * @brief Executes a command in the operating system.
         *
         * @param[in] cmd The command to be executed.
         *
         * @return 0 on success, -1 on error.
         */
        extern int execcmd(const char *cmd);

        /**
         * @brief Expands a file path, including wildcards and time tags.
         *
         * @param[in] path The input file path.
         * @param[out] paths The expanded file paths.
         * @param[in] nmax The maximum number of expanded file paths.
         *
         * @return The number of expanded file paths on success, -1 on error.
         */
        extern int expath(const char *path, char *paths[], int nmax);

        /**
         * @brief Creates a directory with the given path.
         *
         * @param[in] path The path of the directory to be created.
         *
         * @return none
         */
        extern void createdir(const char *path);

        /**
         * @brief Reconstructs the path of a file, replacing the date and time with the specified one
         *
         * This function takes a path string, replaces the date and time component with the provided time,
         * and stores the reconstructed path in the rpath buffer.
         *
         * @param path The original path string
         * @param rpath The buffer to store the reconstructed path
         * @param time The date and time to be used for reconstruction
         * @param rov The Rov identifier (optional)
         * @param base The base identifier (optional)
         *
         * @return 0 if successful, -1 if an error occurred
         */
        extern int reppath(const char *path, char *rpath, gtime_t time, const char *rov, const char *base);

        /**
         * @brief Reconstructs multiple paths of files within a time interval, replacing the date and time
         *
         * This function takes a path string pattern, replaces the date and time component of each matched file
         * within the specified time interval, and stores the reconstructed paths in the rpath array.
         *
         * @param path The path string pattern
         * @param rpath The array to store the reconstructed paths
         * @param nmax The maximum number of paths to store in the rpath array
         * @param ts The start time of the time interval
         * @param te The end time of the time interval
         * @param rov The Rov identifier (optional)
         * @param base The base identifier (optional)
         *
         * @return The number of reconstructed paths stored in the rpath array, or -1 if an error occurred
         */
        extern int reppaths(const char *path, char *rpath[], int nmax, gtime_t ts, gtime_t te, const char *rov, const char *base);

        /**
         * @brief Uncompresses a compressed file
         *
         * This function takes a compressed file path string, uncompresses it, and stores the uncompressed
         * file in the specified uncfile path.
         *
         * @param file The path to the compressed file
         * @param uncfile The path to store the uncompressed file
         *
         * @return 0 if successful, -1 if an error occurred
         */
        extern int uncompress(const char *file, char *uncfile);

    }

#ifdef __cplusplus
}
#endif

#endif //! RTKLIB_TIME_H
