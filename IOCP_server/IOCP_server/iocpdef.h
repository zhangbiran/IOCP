#ifndef __IOCP_DEF_H
#define __IOCP_DEF_H

#include "include.h"
#include "messageBlock.h"
#include "constdef.h"
#include "context.h"

class READ_IO_DATA;
class SEND_IO_DATA;

enum OperType
{
	OperType_Accept = 0,
	OperType_Recv = 1,
	OperType_Send = 2,
};

enum AppType
{
	AppType_Server = 1,
	AppType_Client = 2,
};


class HANDLE_DATA
{
public:
	HANDLE_DATA(SOCKET sock, sockaddr_in addr);
	~HANDLE_DATA();
	SOCKET getSock() { return _sock; }
	sockaddr_in getAddr() { return _addr; }
	
protected:
	SOCKET _sock;
	sockaddr_in _addr;
};

class IO_DATA;

class IO
{
public:
	virtual ~IO() {}
	virtual bool handleEvent(HANDLE_DATA * handle_data, int transfered) = 0;
	virtual void makeWSABuf() = 0;
	virtual bool isFinished() { return true; }
	virtual void onFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData) {}
	virtual void onNoFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData) {}

	WSABUF _wsaBuf;
	int _openType;
};

class READ_IO_DATA : public IO
{
public:
	READ_IO_DATA()
	{
		init();
	}
	~READ_IO_DATA();
	bool handleEvent(HANDLE_DATA * handle_data, int transfered);
	void makeWSABuf();
	bool isFinished();
	void onFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData);
	void onNoFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData);

private:
	void init()
	{
		_openType = OperType_Recv;
	}

public:
	IOCPMessageBlock _msgBlock;
};

class SEND_IO_DATA : public IO
{
public:
	SEND_IO_DATA()
	{
		init();
	}

	bool handleEvent(HANDLE_DATA * handle_data, int transfered);
	bool makeCopy(char *buf, int len);
	void makeWSABuf();
	bool isFinished();
	void onFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData);
	void onNoFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData);
private:
	void init()
	{
		_buf = NULL;
		_len = 0;
		_hasTransfer = 0;
		_openType = OperType_Send;
	}
public:
	char* _buf;
	int _len;
	int _hasTransfer;
};

class IO_DATA
{
public:
	IO_DATA();
	~IO_DATA();

	void zeroOverlapped();
	bool handleEvent(HANDLE_DATA * handle_data, int transfered);

	OVERLAPPED overlapped;
	IO * _io;
};


#endif