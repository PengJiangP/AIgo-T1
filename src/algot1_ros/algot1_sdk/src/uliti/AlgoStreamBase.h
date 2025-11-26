/*
 * File: streambase.h
 * Author: jack <283422622@qq.com>
 * Date: June 30, 2023
 * Version: 1.0
 * Description: This header file contains the definition of an AlgoStreamBase class. it is
 * Copyright 2023 by Algo1010 Technologny Co.,Ltd.
 */

#ifndef _ALGO_STREAM_BASE_H_
#define _ALGO_STREAM_BASE_H_

#include <string>

#define ALGO_DAT_MAX_LEN 40960

enum AlgoStreamType
{
	ALGO_STR_NONE = 0,		/* stream type: none */
	ALGO_STR_SERIAL = 1,	/* stream type: serial */
	ALGO_STR_FILE = 2,		/* stream type: file */
	ALGO_STR_TCPSVR = 3,	/* stream type: TCP server */
	ALGO_STR_TCPCLI = 4,	/* stream type: TCP client */
	ALGO_STR_NTRIPSVR = 6,	/* stream type: NTRIP server */
	ALGO_STR_NTRIPCLI = 7,	/* stream type: NTRIP client */
	ALGO_STR_FTP = 8,		/* stream type: ftp */
	ALGO_STR_HTTP = 9,		/* stream type: http */
	ALGO_STR_NTRIPC_S = 10, /* stream type: NTRIP caster server */
	ALGO_STR_NTRIPC_C = 11, /* stream type: NTRIP caster client */
	ALGO_STR_UDPSVR = 12,	/* stream type: UDP server */
	ALGO_STR_UDPCLI = 13,	/* stream type: UDP server */
	ALGO_STR_FIFO = 14,		/* stream type: FIFO */
	ALGO_STR_CANFD = 15,	/* stream type: Can */
	ALGO_STR_MEMBUF = 16,	/* stream type: memory buffer */
};

class AlgoStreamBase
{
public:
	unsigned char m_startCmd[128]; //ALGO_CMD_MAX_LEN = 128
	int m_startCmdLen = 0;
	int m_enable = 0;

private:
	void *m_stream_t = NULL;
	std::string m_name = "";
	std::string m_path = "";
	int m_nettype;

public:
	AlgoStreamBase();
	~AlgoStreamBase();

	bool init(std::string name, int nettype, std::string path,std::string startCmd="");
	std::string getinfo();
	bool open();
	int write(std::string data);
	int write(unsigned char *buffer, int len);
	std::string read();
	int read(unsigned char *buffer, int maxlen);
	void close();

	static std::string toPath4Net(std::string ip, int port);
	static std::string toPath4Com(std::string name, int bitrate, std::string parity, int bytesize, int stopbits);
	static int netName2netType(std::string netName);
	static std::string netType2netName(int nettype);
};

#endif //_ALGO_STREAM_BASE_H_
