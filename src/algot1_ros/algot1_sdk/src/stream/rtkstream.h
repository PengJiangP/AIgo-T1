#ifndef RTKLIB_STREAM_H
#define RTKLIB_STREAM_H
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
#include "rtktime.h"

    /* constants -----------------------------------------------------------------*/

#define STR_NONE 0      /* stream type: none */
#define STR_SERIAL 1    /* stream type: serial */
#define STR_FILE 2      /* stream type: file */
#define STR_TCPSVR 3    /* stream type: TCP server */
#define STR_TCPCLI 4    /* stream type: TCP client */
#define STR_NTRIPSVR 6  /* stream type: NTRIP server */
#define STR_NTRIPCLI 7  /* stream type: NTRIP client */
#define STR_FTP 8       /* stream type: ftp */
#define STR_HTTP 9      /* stream type: http */
#define STR_NTRIPC_S 10 /* stream type: NTRIP caster server */
#define STR_NTRIPC_C 11 /* stream type: NTRIP caster client */
#define STR_UDPSVR 12   /* stream type: UDP server */
#define STR_UDPCLI 13   /* stream type: UDP server */
#define STR_FIFO 14     /* stream type: FIFO */
#define STR_CANFD 15    /* stream type: CAN */
#define STR_MEMBUF 16   /* stream type: memory buffer */

#define STR_MODE_R 0x1  /* stream mode: read */
#define STR_MODE_W 0x2  /* stream mode: write */
#define STR_MODE_RW 0x3 /* stream mode: read/write */

#define MAXSTRPATH 1024 /* max length of stream path */
#define MAXSTRMSG 1024  /* max length of stream message */

    namespace RTKLIB
    {

        /* type definitions ----------------------------------------------------------*/
        typedef struct
        {                             /* stream type */
            int type;                 /* type (STR_???) */
            int mode;                 /* mode (STR_MODE_?) */
            int state;                /* state (-1:error,0:close,1:open) */
            unsigned int inb, inr;    /* input bytes/rate */
            unsigned int outb, outr;  /* output bytes/rate */
            unsigned int tick, tact;  /* tick/active tick */
            unsigned int inbt, outbt; /* input/output bytes at tick */
            lock_t lock;              /* lock flag */
            void *port;               /* type dependent port control struct */
            char path[MAXSTRPATH];    /* stream path */
            char msg[MAXSTRMSG];      /* stream message */
        } stream_t;

        /* stream data input and output functions ------------------------------------*/
        extern void strinitcom(void);
        extern void strinit(stream_t *stream);
        extern void strlock(stream_t *stream);
        extern void strunlock(stream_t *stream);
        extern int stropen(stream_t *stream, int type, int mode, const char *path);
        extern void strclose(stream_t *stream);
        extern int strread(stream_t *stream, unsigned char *buff, int n);
        extern int strwrite(stream_t *stream, unsigned char *buff, int n);
        extern void strsync(stream_t *stream1, stream_t *stream2);
        extern int strstat(stream_t *stream, char *msg);
        extern void strsum(stream_t *stream, int *inb, int *inr, int *outb, int *outr);
        extern void strsetopt(const int *opt);
        extern gtime_t strgettime(stream_t *stream);
        extern void strsendnmea(stream_t *stream, const double *pos);
        extern void strsendcmd(stream_t *stream, const char *cmd);
        extern void strsettimeout(stream_t *stream, int toinact, int tirecon);
        extern void strsetdir(const char *dir);
        extern void strsetproxy(const char *addr);
        extern int setcbrate(stream_t *str, int brate);

    }

#ifdef __cplusplus
}
#endif
#endif /* RTKLIB_STREAM_H */
