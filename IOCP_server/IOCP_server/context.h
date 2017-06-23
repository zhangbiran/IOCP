#ifndef __IOCP_CONTEXT_H
#define __IOCP_CONTEXT_H

#include "include.h"

class StreamSocket;
class StreamBuf;

class CContextBase
{
public:
	CContextBase(StreamSocket* streamSock);
	virtual ~CContextBase();
	virtual void onConnect() {}
	virtual void onExit() {}
	virtual void onSend() {}
	virtual void onRecv(std::vector<StreamBuf>& streamSeq);
	virtual void send(char* buf, int len);
private:
	StreamSocket* _streamSock;
};

class CContext : public CContextBase
{
public:
	CContext(StreamSocket* streamSock);
	~CContext();
	void onRecv(std::vector<StreamBuf>& streamSeq);
};

#endif