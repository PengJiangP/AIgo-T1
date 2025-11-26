#include "rtktime.h"

#include <stdarg.h>
#include <ctype.h>
#ifndef WIN32
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#define INT_SWAP_TRAC 1

namespace RTKLIB
{

    /* time and string functions -------------------------------------------------*/

    /* constants -----------------------------------------------------------------*/
    const static double gpst0[] = {1980, 1, 6, 0, 0, 0}; /* gps time reference */
    const static double gst0[] = {1999, 8, 22, 0, 0, 0}; /* galileo system time reference */
    const static double bdt0[] = {2006, 1, 1, 0, 0, 0};  /* beidou time reference */

    static double leaps[MAXLEAPS + 1][7] = {/* leap seconds (y,m,d,h,m,s,utc-gpst) */
                                            {2017, 1, 1, 0, 0, 0, -18},
                                            {2015, 7, 1, 0, 0, 0, -17},
                                            {2012, 7, 1, 0, 0, 0, -16},
                                            {2009, 1, 1, 0, 0, 0, -15},
                                            {2006, 1, 1, 0, 0, 0, -14},
                                            {1999, 1, 1, 0, 0, 0, -13},
                                            {1997, 7, 1, 0, 0, 0, -12},
                                            {1996, 1, 1, 0, 0, 0, -11},
                                            {1994, 7, 1, 0, 0, 0, -10},
                                            {1993, 7, 1, 0, 0, 0, -9},
                                            {1992, 7, 1, 0, 0, 0, -8},
                                            {1991, 1, 1, 0, 0, 0, -7},
                                            {1990, 1, 1, 0, 0, 0, -6},
                                            {1988, 1, 1, 0, 0, 0, -5},
                                            {1985, 7, 1, 0, 0, 0, -4},
                                            {1983, 7, 1, 0, 0, 0, -3},
                                            {1982, 7, 1, 0, 0, 0, -2},
                                            {1981, 7, 1, 0, 0, 0, -1},
                                            {0}};

    /* string to number ------------------------------------------------------------
     * convert substring in string to number
     * args   : char   *s        I   string ("... nnn.nnn ...")
     *          int    i,n       I   substring position and width
     * return : converted number (0.0:error)
     *-----------------------------------------------------------------------------*/
    extern double str2num(const char *s, int i, int n)
    {
        double value;
        char str[256], *p = str;

        if (i < 0 || (int)strlen(s) < i || (int)sizeof(str) - 1 < n)
            return 0.0;
        for (s += i; *s && --n >= 0; s++)
            *p++ = *s == 'd' || *s == 'D' ? 'E' : *s;
        *p = '\0';
        return sscanf(str, "%lf", &value) == 1 ? value : 0.0;
    }
    /* string to time --------------------------------------------------------------
     * convert substring in string to gtime_t struct
     * args   : char   *s        I   string ("... yyyy mm dd hh mm ss ...")
     *          int    i,n       I   substring position and width
     *          gtime_t *t       O   gtime_t struct
     * return : status (0:ok,0>:error)
     *-----------------------------------------------------------------------------*/
    extern int str2time(const char *s, int i, int n, gtime_t *t)
    {
        double ep[6];
        char str[256], *p = str;

        if (i < 0 || (int)strlen(s) < i || (int)sizeof(str) - 1 < i)
            return -1;
        for (s += i; *s && --n >= 0;)
            *p++ = *s++;
        *p = '\0';
        if (sscanf(str, "%lf %lf %lf %lf %lf %lf", ep, ep + 1, ep + 2, ep + 3, ep + 4, ep + 5) < 6)
            return -1;
        if (ep[0] < 100.0)
            ep[0] += ep[0] < 80.0 ? 2000.0 : 1900.0;
        *t = epoch2time(ep);
        return 0;
    }
    /* convert calendar day/time to time -------------------------------------------
     * convert calendar day/time to gtime_t struct
     * args   : double *ep       I   day/time {year,month,day,hour,min,sec}
     * return : gtime_t struct
     * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
     *-----------------------------------------------------------------------------*/
    extern gtime_t epoch2time(const double *ep)
    {
        const int doy[] = {1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
        gtime_t time = {0};
        int days, sec, year = (int)ep[0], mon = (int)ep[1], day = (int)ep[2];

        if (year < 1970 || 2099 < year || mon < 1 || 12 < mon)
            return time;

        /* leap year if year%4==0 in 1901-2099 */
        days = (year - 1970) * 365 + (year - 1969) / 4 + doy[mon - 1] + day - 2 + (year % 4 == 0 && mon >= 3 ? 1 : 0);
        sec = (int)floor(ep[5]);
        time.time = (time_t)days * 86400 + (int)ep[3] * 3600 + (int)ep[4] * 60 + sec;
        time.sec = ep[5] - sec;
        return time;
    }
    /* time to calendar day/time ---------------------------------------------------
     * convert gtime_t struct to calendar day/time
     * args   : gtime_t t        I   gtime_t struct
     *          double *ep       O   day/time {year,month,day,hour,min,sec}
     * return : none
     * notes  : proper in 1970-2037 or 1970-2099 (64bit time_t)
     *-----------------------------------------------------------------------------*/
    extern void time2epoch(gtime_t t, double *ep)
    {
        const int mday[] = {/* # of days in a month */
                            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
                            31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int days, sec, mon, day;

        /* leap year if year%4==0 in 1901-2099 */
        days = (int)(t.time / 86400);
        sec = (int)(t.time - (time_t)days * 86400);
        for (day = days % 1461, mon = 0; mon < 48; mon++)
        {
            if (day >= mday[mon])
                day -= mday[mon];
            else
                break;
        }
        ep[0] = 1970 + days / 1461 * 4 + mon / 12;
        ep[1] = mon % 12 + 1;
        ep[2] = day + 1;
        ep[3] = sec / 3600;
        ep[4] = sec % 3600 / 60;
        ep[5] = sec % 60 + t.sec;
    }
    /* gps time to time ------------------------------------------------------------
     * convert week and tow in gps time to gtime_t struct
     * args   : int    week      I   week number in gps time
     *          double sec       I   time of week in gps time (s)
     * return : gtime_t struct
     *-----------------------------------------------------------------------------*/
    extern gtime_t gpst2time(int week, double sec)
    {
        gtime_t t = epoch2time(gpst0);

        if (sec < -1E9 || 1E9 < sec)
            sec = 0.0;
        t.time += 86400 * 7 * week + (int)sec;
        t.sec = sec - (int)sec;
        return t;
    }
    /* time to gps time ------------------------------------------------------------
     * convert gtime_t struct to week and tow in gps time
     * args   : gtime_t t        I   gtime_t struct
     *          int    *week     IO  week number in gps time (NULL: no output)
     * return : time of week in gps time (s)
     *-----------------------------------------------------------------------------*/
    extern double time2gpst(gtime_t t, int *week)
    {
        gtime_t t0 = epoch2time(gpst0);
        time_t sec = t.time - t0.time;
        int w = (int)(sec / (86400 * 7));

        if (week)
            *week = w;
        return (double)(sec - w * 86400 * 7) + t.sec;
    }
    /* galileo system time to time -------------------------------------------------
     * convert week and tow in galileo system time (gst) to gtime_t struct
     * args   : int    week      I   week number in gst
     *          double sec       I   time of week in gst (s)
     * return : gtime_t struct
     *-----------------------------------------------------------------------------*/
    extern gtime_t gst2time(int week, double sec)
    {
        gtime_t t = epoch2time(gst0);

        if (sec < -1E9 || 1E9 < sec)
            sec = 0.0;
        t.time += 86400 * 7 * week + (int)sec;
        t.sec = sec - (int)sec;
        return t;
    }
    /* time to galileo system time -------------------------------------------------
     * convert gtime_t struct to week and tow in galileo system time (gst)
     * args   : gtime_t t        I   gtime_t struct
     *          int    *week     IO  week number in gst (NULL: no output)
     * return : time of week in gst (s)
     *-----------------------------------------------------------------------------*/
    extern double time2gst(gtime_t t, int *week)
    {
        gtime_t t0 = epoch2time(gst0);
        time_t sec = t.time - t0.time;
        int w = (int)(sec / (86400 * 7));

        if (week)
            *week = w;
        return (double)(sec - w * 86400 * 7) + t.sec;
    }
    /* beidou time (bdt) to time ---------------------------------------------------
     * convert week and tow in beidou time (bdt) to gtime_t struct
     * args   : int    week      I   week number in bdt
     *          double sec       I   time of week in bdt (s)
     * return : gtime_t struct
     *-----------------------------------------------------------------------------*/
    extern gtime_t bdt2time(int week, double sec)
    {
        gtime_t t = epoch2time(bdt0);

        if (sec < -1E9 || 1E9 < sec)
            sec = 0.0;
        t.time += 86400 * 7 * week + (int)sec;
        t.sec = sec - (int)sec;
        return t;
    }
    /* time to beidouo time (bdt) --------------------------------------------------
     * convert gtime_t struct to week and tow in beidou time (bdt)
     * args   : gtime_t t        I   gtime_t struct
     *          int    *week     IO  week number in bdt (NULL: no output)
     * return : time of week in bdt (s)
     *-----------------------------------------------------------------------------*/
    extern double time2bdt(gtime_t t, int *week)
    {
        gtime_t t0 = epoch2time(bdt0);
        time_t sec = t.time - t0.time;
        int w = (int)(sec / (86400 * 7));

        if (week)
            *week = w;
        return (double)(sec - w * 86400 * 7) + t.sec;
    }
    /* add time --------------------------------------------------------------------
     * add time to gtime_t struct
     * args   : gtime_t t        I   gtime_t struct
     *          double sec       I   time to add (s)
     * return : gtime_t struct (t+sec)
     *-----------------------------------------------------------------------------*/
    extern gtime_t timeadd(gtime_t t, double sec)
    {
        double tt;

        t.sec += sec;
        tt = floor(t.sec);
        t.time += (int)tt;
        t.sec -= tt;
        return t;
    }
    /* time difference -------------------------------------------------------------
     * difference between gtime_t structs
     * args   : gtime_t t1,t2    I   gtime_t structs
     * return : time difference (t1-t2) (s)
     *-----------------------------------------------------------------------------*/
    extern double timediff(gtime_t t1, gtime_t t2)
    {
        return difftime(t1.time, t2.time) + t1.sec - t2.sec;
    }
    /* get current time in utc -----------------------------------------------------
     * get current time in utc
     * args   : none
     * return : current time in utc
     *-----------------------------------------------------------------------------*/
    static double timeoffset_ = 0.0; /* time offset (s) */

    extern gtime_t timeget(void)
    {
        double ep[6] = {0};
#ifdef WIN32
        SYSTEMTIME ts;

        GetSystemTime(&ts); /* utc */
        ep[0] = ts.wYear;
        ep[1] = ts.wMonth;
        ep[2] = ts.wDay;
        ep[3] = ts.wHour;
        ep[4] = ts.wMinute;
        ep[5] = ts.wSecond + ts.wMilliseconds * 1E-3;
#else
        struct timeval tv;
        struct tm *tt;

        if (!gettimeofday(&tv, NULL) && (tt = gmtime(&tv.tv_sec)))
        {
            ep[0] = tt->tm_year + 1900;
            ep[1] = tt->tm_mon + 1;
            ep[2] = tt->tm_mday;
            ep[3] = tt->tm_hour;
            ep[4] = tt->tm_min;
            ep[5] = tt->tm_sec + tv.tv_usec * 1E-6;
        }
#endif
        return timeadd(epoch2time(ep), timeoffset_);
    }
    /* set current time in utc -----------------------------------------------------
     * set current time in utc
     * args   : gtime_t          I   current time in utc
     * return : none
     * notes  : just set time offset between cpu time and current time
     *          the time offset is reflected to only timeget()
     *          not reentrant
     *-----------------------------------------------------------------------------*/
    extern void timeset(gtime_t t)
    {
        timeoffset_ += timediff(t, timeget());
    }
    /* read leap seconds table -----------------------------------------------------
     * read leap seconds table
     * args   : char    *file    I   leap seconds table file
     * return : status (1:ok,0:error)
     * notes  : (1) The records in the table file cosist of the following fields:
     *              year month day hour min sec UTC-GPST(s)
     *          (2) The date and time indicate the start UTC time for the UTC-GPST
     *          (3) The date and time should be descending order.
     *-----------------------------------------------------------------------------*/
    extern int read_leaps(const char *file)
    {
        FILE *fp;
        char buff[256], *p;
        int i, n = 0, ep[6], ls;

        if (!(fp = fopen(file, "r")))
            return 0;

        while (fgets(buff, sizeof(buff), fp) && n < MAXLEAPS)
        {
            if ((p = strchr(buff, '#')))
                *p = '\0';
            if (sscanf(buff, "%d %d %d %d %d %d %d", ep, ep + 1, ep + 2, ep + 3, ep + 4, ep + 5,
                       &ls) < 7)
                continue;
            for (i = 0; i < 6; i++)
                leaps[n][i] = ep[i];
            leaps[n++][6] = ls;
        }
        for (i = 0; i < 7; i++)
            leaps[n][i] = 0.0;
        fclose(fp);
        return 1;
    }
    /* gpstime to utc --------------------------------------------------------------
     * convert gpstime to utc considering leap seconds
     * args   : gtime_t t        I   time expressed in gpstime
     * return : time expressed in utc
     * notes  : ignore slight time offset under 100 ns
     *-----------------------------------------------------------------------------*/
    extern gtime_t gpst2utc(gtime_t t)
    {
        gtime_t tu;
        int i;

        for (i = 0; leaps[i][0] > 0; i++)
        {
            tu = timeadd(t, leaps[i][6]);
            if (timediff(tu, epoch2time(leaps[i])) >= 0.0)
                return tu;
        }
        return t;
    }
    /* utc to gpstime --------------------------------------------------------------
     * convert utc to gpstime considering leap seconds
     * args   : gtime_t t        I   time expressed in utc
     * return : time expressed in gpstime
     * notes  : ignore slight time offset under 100 ns
     *-----------------------------------------------------------------------------*/
    extern gtime_t utc2gpst(gtime_t t)
    {
        int i;

        for (i = 0; leaps[i][0] > 0; i++)
        {
            if (timediff(t, epoch2time(leaps[i])) >= 0.0)
                return timeadd(t, -leaps[i][6]);
        }
        return t;
    }
    /* gpstime to bdt --------------------------------------------------------------
     * convert gpstime to bdt (beidou navigation satellite system time)
     * args   : gtime_t t        I   time expressed in gpstime
     * return : time expressed in bdt
     * notes  : ref [8] 3.3, 2006/1/1 00:00 BDT = 2006/1/1 00:00 UTC
     *          no leap seconds in BDT
     *          ignore slight time offset under 100 ns
     *-----------------------------------------------------------------------------*/
    extern gtime_t gpst2bdt(gtime_t t)
    {
        return timeadd(t, -14.0);
    }
    /* bdt to gpstime --------------------------------------------------------------
     * convert bdt (beidou navigation satellite system time) to gpstime
     * args   : gtime_t t        I   time expressed in bdt
     * return : time expressed in gpstime
     * notes  : see gpst2bdt()
     *-----------------------------------------------------------------------------*/
    extern gtime_t bdt2gpst(gtime_t t)
    {
        return timeadd(t, 14.0);
    }
    /* time to day and sec -------------------------------------------------------*/
    static double time2sec(gtime_t time, gtime_t *day)
    {
        double ep[6], sec;
        time2epoch(time, ep);
        sec = ep[3] * 3600.0 + ep[4] * 60.0 + ep[5];
        ep[3] = ep[4] = ep[5] = 0.0;
        *day = epoch2time(ep);
        return sec;
    }
    /* utc to gmst -----------------------------------------------------------------
     * convert utc to gmst (Greenwich mean sidereal time)
     * args   : gtime_t t        I   time expressed in utc
     *          double ut1_utc   I   UT1-UTC (s)
     * return : gmst (rad)
     *-----------------------------------------------------------------------------*/
    extern double utc2gmst(gtime_t t, double ut1_utc)
    {
        const double ep2000[] = {2000, 1, 1, 12, 0, 0};
        gtime_t tut, tut0;
        double ut, t1, t2, t3, gmst0, gmst;

        tut = timeadd(t, ut1_utc);
        ut = time2sec(tut, &tut0);
        t1 = timediff(tut0, epoch2time(ep2000)) / 86400.0 / 36525.0;
        t2 = t1 * t1;
        t3 = t2 * t1;
        gmst0 = 24110.54841 + 8640184.812866 * t1 + 0.093104 * t2 - 6.2E-6 * t3;
        gmst = gmst0 + 1.002737909350795 * ut;

        return fmod(gmst, 86400.0) * PI / 43200.0; /* 0 <= gmst <= 2*PI */
    }
    /* time to string --------------------------------------------------------------
     * convert gtime_t struct to string
     * args   : gtime_t t        I   gtime_t struct
     *          char   *s        O   string ("yyyy/mm/dd hh:mm:ss.ssss")
     *          int    n         I   number of decimals
     * return : none
     *-----------------------------------------------------------------------------*/
    extern void time2str(gtime_t t, char *s, int n)
    {
        double ep[6];

        if (n < 0)
            n = 0;
        else if (n > 12)
            n = 12;
        if (1.0 - t.sec < 0.5 / pow(10.0, n))
        {
            t.time++;
            t.sec = 0.0;
        };
        time2epoch(t, ep);
        sprintf(s, "%04.0f/%02.0f/%02.0f %02.0f:%02.0f:%0*.*f", ep[0], ep[1], ep[2],
                ep[3], ep[4], n <= 0 ? 2 : n + 3, n <= 0 ? 0 : n, ep[5]);
    }
    /* get time string -------------------------------------------------------------
     * get time string
     * args   : gtime_t t        I   gtime_t struct
     *          int    n         I   number of decimals
     * return : time string
     * notes  : not reentrant, do not use multiple in a function
     *-----------------------------------------------------------------------------*/
    extern char *time_str(gtime_t t, int n)
    {
        static char buff[64];
        time2str(t, buff, n);
        return buff;
    }
    /* time to day of year ---------------------------------------------------------
     * convert time to day of year
     * args   : gtime_t t        I   gtime_t struct
     * return : day of year (days)
     *-----------------------------------------------------------------------------*/
    extern double time2doy(gtime_t t)
    {
        double ep[6];

        time2epoch(t, ep);
        ep[1] = ep[2] = 1.0;
        ep[3] = ep[4] = ep[5] = 0.0;
        return timediff(t, epoch2time(ep)) / 86400.0 + 1.0;
    }
    /* adjust gps week number ------------------------------------------------------
     * adjust gps week number using cpu time
     * args   : int   week       I   not-adjusted gps week number
     * return : adjusted gps week number
     *-----------------------------------------------------------------------------*/
    extern int adjgpsweek(int week)
    {
        int w;
        (void)time2gpst(utc2gpst(timeget()), &w);
        if (w < 1560)
            w = 1560; /* use 2009/12/1 if time is earlier than 2009/12/1 */
        return week + (w - week + 512) / 1024 * 1024;
    }
    /* get tick time ---------------------------------------------------------------
     * get current tick in ms
     * args   : none
     * return : current tick in ms
     *-----------------------------------------------------------------------------*/
    extern unsigned int tickget(void)
    {
#ifdef WIN32
        return (unsigned int)timeGetTime();
#else
        struct timespec tp = {0};
        struct timeval tv = {0};

#ifdef CLOCK_MONOTONIC_RAW
        /* linux kernel > 2.6.28 */
        if (!clock_gettime(CLOCK_MONOTONIC_RAW, &tp))
        {
            return tp.tv_sec * 1000u + tp.tv_nsec / 1000000u;
        }
        else
        {
            gettimeofday(&tv, NULL);
            return tv.tv_sec * 1000u + tv.tv_usec / 1000u;
        }
#else
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000u + tv.tv_usec / 1000u;
#endif
#endif /* WIN32 */
    }
    /* sleep ms --------------------------------------------------------------------
     * sleep ms
     * args   : int   ms         I   miliseconds to sleep (<0:no sleep)
     * return : none
     *-----------------------------------------------------------------------------*/
    extern void sleepms(int ms)
    {
#ifdef WIN32
        if (ms < 5)
            Sleep(1);
        else
            Sleep(ms);
#else
        struct timespec ts;
        if (ms <= 0)
            return;
        ts.tv_sec = (time_t)(ms / 1000);
        ts.tv_nsec = (long)(ms % 1000 * 1000000);
        nanosleep(&ts, NULL);
#endif
    }

/* debug trace functions -----------------------------------------------------*/
#ifdef TRACE

    static FILE *fp_trace = NULL;       /* file pointer of trace */
    static char file_trace[1024];       /* trace file */
    static int level_trace = 0;         /* level of trace */
    static unsigned int tick_trace = 0; /* tick time at traceopen (ms) */
    static gtime_t time_trace = {0};    /* time at traceopen */
    static lock_t lock_trace;           /* lock for trace */

    static void traceswap(void)
    {
        gtime_t time = utc2gpst(timeget());
        char path[1024];

        lock(&lock_trace);

        if ((int)(time2gpst(time, NULL) / INT_SWAP_TRAC) ==
            (int)(time2gpst(time_trace, NULL) / INT_SWAP_TRAC))
        {
            unlock(&lock_trace);
            return;
        }
        time_trace = time;

        if (!reppath(file_trace, path, time, "", ""))
        {
            unlock(&lock_trace);
            return;
        }
        if (fp_trace)
            fclose(fp_trace);

        if (!(fp_trace = fopen(path, "w")))
        {
            fp_trace = stderr;
        }
        unlock(&lock_trace);
    }
    extern void traceopen(const char *file)
    {
        gtime_t time = utc2gpst(timeget());
        char path[1024];

        reppath(file, path, time, "", "");
        if (!*path || !(fp_trace = fopen(path, "w")))
            fp_trace = stderr;
        strcpy(file_trace, file);
        tick_trace = tickget();
        time_trace = time;
        initlock(&lock_trace);
    }
    extern void traceclose(void)
    {
        if (fp_trace && fp_trace != stderr)
            fclose(fp_trace);
        fp_trace = NULL;
        file_trace[0] = '\0';
    }
    extern void tracelevel(int level)
    {
        level_trace = level;
    }
    extern void trace(int level, const char *format, ...)
    {
        va_list ap;

        /* print error message to stderr */
        if (level <= 1)
        {
            va_start(ap, format);
            vfprintf(stderr, format, ap);
            va_end(ap);
        }
        if (!fp_trace || level > level_trace)
            return;
        traceswap();
        fprintf(fp_trace, "%d ", level);
        va_start(ap, format);
        vfprintf(fp_trace, format, ap);
        va_end(ap);
        fflush(fp_trace);
    }
    extern void tracet(int level, const char *format, ...)
    {
        va_list ap;

        if (!fp_trace || level > level_trace)
            return;
        traceswap();
        fprintf(fp_trace, "%d %9.3f: ", level, (tickget() - tick_trace) / 1000.0);
        va_start(ap, format);
        vfprintf(fp_trace, format, ap);
        va_end(ap);
        fflush(fp_trace);
    }
    extern void tracemat(int level, const double *A, int n, int m, int p, int q)
    {
        if (!fp_trace || level > level_trace)
            return;
        // matfprint(A, n, m, p, q, fp_trace);
        fflush(fp_trace);
    }

    extern void traceb(int level, const unsigned char *p, int n)
    {
        int i;
        if (!fp_trace || level > level_trace)
            return;
        for (i = 0; i < n; i++)
            fprintf(fp_trace, "%02X%s", *p++, i % 8 == 7 ? " " : "");
        fprintf(fp_trace, "\n");
    }
#else
    extern void traceopen(const char *file) {}
    extern void traceclose(void) {}
    extern void tracelevel(int level) {}
    extern void trace(int level, const char *format, ...) {}
    extern void tracet(int level, const char *format, ...) {}
    extern void tracemat(int level, const double *A, int n, int m, int p, int q) {}
    extern void traceb(int level, const unsigned char *p, int n) {}

#endif /* TRACE */

    /* execute command -------------------------------------------------------------
     * execute command line by operating system shell
     * args   : char   *cmd      I   command line
     * return : execution status (0:ok,0>:error)
     *-----------------------------------------------------------------------------*/
    extern int execcmd(const char *cmd)
    {
#ifdef WIN32
        PROCESS_INFORMATION info;
        STARTUPINFO si = {0};
        DWORD stat;
        char cmds[1024];

        trace(3, "execcmd: cmd=%s\n", cmd);

        si.cb = sizeof(si);
        sprintf(cmds, "cmd /c %s", cmd);
        if (!CreateProcess(NULL, (LPTSTR)cmds, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL,
                           NULL, &si, &info))
            return -1;
        WaitForSingleObject(info.hProcess, INFINITE);
        if (!GetExitCodeProcess(info.hProcess, &stat))
            stat = -1;
        CloseHandle(info.hProcess);
        CloseHandle(info.hThread);
        return (int)stat;
#else
        trace(3, "execcmd: cmd=%s\n", cmd);

        return system(cmd);
#endif
    }
    /* expand file path ------------------------------------------------------------
     * expand file path with wild-card (*) in file
     * args   : char   *path     I   file path to expand (captal insensitive)
     *          char   *paths    O   expanded file paths
     *          int    nmax      I   max number of expanded file paths
     * return : number of expanded file paths
     * notes  : the order of expanded files is alphabetical order
     *-----------------------------------------------------------------------------*/
    extern int expath(const char *path, char *paths[], int nmax)
    {
        int i, j, n = 0;
        char tmp[1024];
#ifdef WIN32
        WIN32_FIND_DATA file;
        HANDLE h;
        char dir[1024] = "", *p;

        trace(3, "expath  : path=%s nmax=%d\n", path, nmax);

        if ((p = strrchr(path, '\\')))
        {
            strncpy(dir, path, p - path + 1);
            dir[p - path + 1] = '\0';
        }
        if ((h = FindFirstFile((LPCTSTR)path, &file)) == INVALID_HANDLE_VALUE)
        {
            strcpy(paths[0], path);
            return 1;
        }
        sprintf(paths[n++], "%s%s", dir, file.cFileName);
        while (FindNextFile(h, &file) && n < nmax)
        {
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                continue;
            sprintf(paths[n++], "%s%s", dir, file.cFileName);
        }
        FindClose(h);
#else
        struct dirent *d;
        DIR *dp;
        const char *file = path;
        char dir[1024] = "", s1[1024], s2[1024], *p, *q, *r;

        trace(3, "expath  : path=%s nmax=%d\n", path, nmax);

        if ((p = (char *)strrchr(path, '/')) || (p = (char *)strrchr(path, '\\')))
        {
            file = p + 1;
            strncpy(dir, path, p - path + 1);
            dir[p - path + 1] = '\0';
        }

        if (!(dp = opendir(*dir ? dir : ".")))
            return 0;
        while ((d = readdir(dp)))
        {
            if (*(d->d_name) == '.')
                continue;
            sprintf(s1, "^%s$", d->d_name);
            sprintf(s2, "^%s$", file);
            for (p = s1; *p; p++)
                *p = (char)tolower((int)*p);
            for (p = s2; *p; p++)
                *p = (char)tolower((int)*p);

            for (p = s1, q = strtok_r(s2, "*", &r); q; q = strtok_r(NULL, "*", &r))
            {
                if ((p = strstr(p, q)))
                    p += strlen(q);
                else
                    break;
            }
            if (p && n < nmax)
                sprintf(paths[n++], "%s%s", dir, d->d_name);
        }
        closedir(dp);
#endif
        /* sort paths in alphabetical order */
        for (i = 0; i < n - 1; i++)
        {
            for (j = i + 1; j < n; j++)
            {
                if (strcmp(paths[i], paths[j]) > 0)
                {
                    strcpy(tmp, paths[i]);
                    strcpy(paths[i], paths[j]);
                    strcpy(paths[j], tmp);
                }
            }
        }
        for (i = 0; i < n; i++)
            trace(3, "expath  : file=%s\n", paths[i]);

        return n;
    }

    /* create directory ------------------------------------------------------------
     * create directory if not exist
     * args   : char   *path     I   file path to be saved
     * return : none
     * notes  : not recursive. only one level
     *-----------------------------------------------------------------------------*/
    extern void createdir(const char *path)
    {
        char buff[1024], *p;

        tracet(3, "createdir: path=%s\n", path);

        strcpy(buff, path);
        if (!(p = strrchr(buff, FILEPATHSEP)))
            return;
        *p = '\0';

#ifdef WIN32
        CreateDirectory(buff, NULL);
#else
        mkdir(buff, 0777);
#endif
    }

    /* replace string ------------------------------------------------------------*/
    static int repstr(char *str, const char *pat, const char *rep)
    {
        int len = strlen(pat);
        char buff[1024], *p, *q, *r;

        for (p = str, r = buff; *p; p = q + len)
        {
            if (!(q = strstr(p, pat)))
                break;
            strncpy(r, p, q - p);
            r += q - p;
            r += sprintf(r, "%s", rep);
        }
        if (p <= str)
            return 0;
        strcpy(r, p);
        strcpy(str, buff);
        return 1;
    }
    /* replace keywords in file path -----------------------------------------------
     * replace keywords in file path with date, time, rover and base station id
     * args   : char   *path     I   file path (see below)
     *          char   *rpath    O   file path in which keywords replaced (see below)
     *          gtime_t time     I   time (gpst)  (time.time==0: not replaced)
     *          char   *rov      I   rover id string        ("": not replaced)
     *          char   *base     I   base station id string ("": not replaced)
     * return : status (1:keywords replaced, 0:no valid keyword in the path,
     *                  -1:no valid time)
     * notes  : the following keywords in path are replaced by date, time and name
     *              %Y -> yyyy : year (4 digits) (1900-2099)
     *              %y -> yy   : year (2 digits) (00-99)
     *              %m -> mm   : month           (01-12)
     *              %d -> dd   : day of month    (01-31)
     *              %h -> hh   : hours           (00-23)
     *              %M -> mm   : minutes         (00-59)
     *              %S -> ss   : seconds         (00-59)
     *              %n -> ddd  : day of year     (001-366)
     *              %W -> wwww : gps week        (0001-9999)
     *              %D -> d    : day of gps week (0-6)
     *              %H -> h    : hour code       (a=0,b=1,c=2,...,x=23)
     *              %ha-> hh   : 3 hours         (00,03,06,...,21)
     *              %hb-> hh   : 6 hours         (00,06,12,18)
     *              %hc-> hh   : 12 hours        (00,12)
     *              %t -> mm   : 15 minutes      (00,15,30,45)
     *              %r -> rrrr : rover id
     *              %b -> bbbb : base station id
     *-----------------------------------------------------------------------------*/
    extern int reppath(const char *path, char *rpath, gtime_t time, const char *rov,
                       const char *base)
    {
        double ep[6], ep0[6] = {2000, 1, 1, 0, 0, 0};
        int week, dow, doy, stat = 0;
        char rep[64];

        trace(3, "reppath : path =%s time=%s rov=%s base=%s\n", path, time_str(time, 0),
              rov, base);

        strcpy(rpath, path);

        if (!strstr(rpath, "%"))
            return 0;
        if (*rov)
            stat |= repstr(rpath, "%r", rov);
        if (*base)
            stat |= repstr(rpath, "%b", base);
        if (time.time != 0)
        {
            time2epoch(time, ep);
            ep0[0] = ep[0];
            dow = (int)floor(time2gpst(time, &week) / 86400.0);
            doy = (int)floor(timediff(time, epoch2time(ep0)) / 86400.0) + 1;
            sprintf(rep, "%02d", ((int)ep[3] / 3) * 3);
            stat |= repstr(rpath, "%ha", rep);
            sprintf(rep, "%02d", ((int)ep[3] / 6) * 6);
            stat |= repstr(rpath, "%hb", rep);
            sprintf(rep, "%02d", ((int)ep[3] / 12) * 12);
            stat |= repstr(rpath, "%hc", rep);
            sprintf(rep, "%04.0f", ep[0]);
            stat |= repstr(rpath, "%Y", rep);
            sprintf(rep, "%02.0f", fmod(ep[0], 100.0));
            stat |= repstr(rpath, "%y", rep);
            sprintf(rep, "%02.0f", ep[1]);
            stat |= repstr(rpath, "%m", rep);
            sprintf(rep, "%02.0f", ep[2]);
            stat |= repstr(rpath, "%d", rep);
            sprintf(rep, "%02.0f", ep[3]);
            stat |= repstr(rpath, "%h", rep);
            sprintf(rep, "%02.0f", ep[4]);
            stat |= repstr(rpath, "%M", rep);
            sprintf(rep, "%02.0f", floor(ep[5]));
            stat |= repstr(rpath, "%S", rep);
            sprintf(rep, "%03d", doy);
            stat |= repstr(rpath, "%n", rep);
            sprintf(rep, "%04d", week);
            stat |= repstr(rpath, "%W", rep);
            sprintf(rep, "%d", dow);
            stat |= repstr(rpath, "%D", rep);
            sprintf(rep, "%c", 'a' + (int)ep[3]);
            stat |= repstr(rpath, "%H", rep);
            sprintf(rep, "%02d", ((int)ep[4] / 15) * 15);
            stat |= repstr(rpath, "%t", rep);
        }
        else if (strstr(rpath, "%ha") || strstr(rpath, "%hb") || strstr(rpath, "%hc") ||
                 strstr(rpath, "%Y") || strstr(rpath, "%y") || strstr(rpath, "%m") ||
                 strstr(rpath, "%d") || strstr(rpath, "%h") || strstr(rpath, "%M") ||
                 strstr(rpath, "%S") || strstr(rpath, "%n") || strstr(rpath, "%W") ||
                 strstr(rpath, "%D") || strstr(rpath, "%H") || strstr(rpath, "%t"))
        {
            return -1; /* no valid time */
        }
        trace(3, "reppath : rpath=%s\n", rpath);
        return stat;
    }
    /* replace keywords in file path and generate multiple paths -------------------
     * replace keywords in file path with date, time, rover and base station id
     * generate multiple keywords-replaced paths
     * args   : char   *path     I   file path (see below)
     *          char   *rpath[]  O   file paths in which keywords replaced
     *          int    nmax      I   max number of output file paths
     *          gtime_t ts       I   time start (gpst)
     *          gtime_t te       I   time end   (gpst)
     *          char   *rov      I   rover id string        ("": not replaced)
     *          char   *base     I   base station id string ("": not replaced)
     * return : number of replaced file paths
     * notes  : see reppath() for replacements of keywords.
     *          minimum interval of time replaced is 900s.
     *-----------------------------------------------------------------------------*/
    extern int reppaths(const char *path, char *rpath[], int nmax, gtime_t ts,
                        gtime_t te, const char *rov, const char *base)
    {
        gtime_t time;
        double tow, tint = 86400.0;
        int i, n = 0, week;

        trace(3, "reppaths: path =%s nmax=%d rov=%s base=%s\n", path, nmax, rov, base);

        if (ts.time == 0 || te.time == 0 || timediff(ts, te) > 0.0)
            return 0;

        if (strstr(path, "%S") || strstr(path, "%M") || strstr(path, "%t"))
            tint = 900.0;
        else if (strstr(path, "%h") || strstr(path, "%H"))
            tint = 3600.0;

        tow = time2gpst(ts, &week);
        time = gpst2time(week, floor(tow / tint) * tint);

        while (timediff(time, te) <= 0.0 && n < nmax)
        {
            reppath(path, rpath[n], time, rov, base);
            if (n == 0 || strcmp(rpath[n], rpath[n - 1]))
                n++;
            time = timeadd(time, tint);
        }
        for (i = 0; i < n; i++)
            trace(3, "reppaths: rpath=%s\n", rpath[i]);
        return n;
    }

    /* uncompress file -------------------------------------------------------------
     * uncompress (uncompress/unzip/uncompact hatanaka-compression/tar) file
     * args   : char   *file     I   input file
     *          char   *uncfile  O   uncompressed file
     * return : status (-1:error,0:not compressed file,1:uncompress completed)
     * note   : creates uncompressed file in tempolary directory
     *          gzip and crx2rnx commands have to be installed in commands path
     *-----------------------------------------------------------------------------*/
    extern int uncompress(const char *file, char *uncfile)
    {
        int stat = 0;
        char *p, cmd[2048] = "", tmpfile[1024] = "", buff[1024], *fname, *dir = "";

        trace(3, "uncompress: file=%s\n", file);

        strcpy(tmpfile, file);
        if (!(p = strrchr(tmpfile, '.')))
            return 0;

        /* uncompress by gzip */
        if (!strcmp(p, ".z") || !strcmp(p, ".Z") ||
            !strcmp(p, ".gz") || !strcmp(p, ".GZ") ||
            !strcmp(p, ".zip") || !strcmp(p, ".ZIP"))
        {

            strcpy(uncfile, tmpfile);
            uncfile[p - tmpfile] = '\0';
            sprintf(cmd, "gzip -f -d -c \"%s\" > \"%s\"", tmpfile, uncfile);

            if (execcmd(cmd))
            {
                remove(uncfile);
                return -1;
            }
            strcpy(tmpfile, uncfile);
            stat = 1;
        }
        /* extract tar file */
        if ((p = strrchr(tmpfile, '.')) && !strcmp(p, ".tar"))
        {

            strcpy(uncfile, tmpfile);
            uncfile[p - tmpfile] = '\0';
            strcpy(buff, tmpfile);
            fname = buff;
#ifdef WIN32
            if ((p = strrchr(buff, '\\')))
            {
                *p = '\0';
                dir = fname;
                fname = p + 1;
            }
            sprintf(cmd, "set PATH=%%CD%%;%%PATH%% & cd /D \"%s\" & tar -xf \"%s\"",
                    dir, fname);
#else
            if ((p = strrchr(buff, '/')))
            {
                *p = '\0';
                dir = fname;
                fname = p + 1;
            }
            sprintf(cmd, "tar -C \"%s\" -xf \"%s\"", dir, tmpfile);
#endif
            if (execcmd(cmd))
            {
                if (stat)
                    remove(tmpfile);
                return -1;
            }
            if (stat)
                remove(tmpfile);
            stat = 1;
        }
        /* extract hatanaka-compressed file by cnx2rnx */
        else if ((p = strrchr(tmpfile, '.')) && strlen(p) > 3 && (*(p + 3) == 'd' || *(p + 3) == 'D'))
        {

            strcpy(uncfile, tmpfile);
            uncfile[p - tmpfile + 3] = *(p + 3) == 'D' ? 'O' : 'o';
            sprintf(cmd, "crx2rnx < \"%s\" > \"%s\"", tmpfile, uncfile);

            if (execcmd(cmd))
            {
                remove(uncfile);
                if (stat)
                    remove(tmpfile);
                return -1;
            }
            if (stat)
                remove(tmpfile);
            stat = 1;
        }
        trace(3, "uncompress: stat=%d\n", stat);
        return stat;
    }

}
