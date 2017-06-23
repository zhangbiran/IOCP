#ifndef __IOCP_SERVER_H
#define __IOCP_SERVER_H
#include "include.h"
#include "socket.h"

#include "iocpevent.h"

class StreamSocket;
class AcceptSocket;


class IOCP
{
public:
	IOCP();
	~IOCP();
	bool isInited() { return NULL != _handle; }
	
	bool add(StreamSocket& streamSocket, HANDLE_DATA *handleData);
	bool add(StreamSocket* streamSocket, HANDLE_DATA *handleData);
	bool add(AcceptSocket& acceptSocket, HANDLE_DATA *handleData);
	bool add(AcceptSocket* acceptSocket, HANDLE_DATA *handleData);

	bool getQueuedCompletionStatus();

private:
	bool add(HANDLE sock, ULONG_PTR key);

private:
	HANDLE _handle;
	IOCPEvent *_iocpEvent;
};


#endif