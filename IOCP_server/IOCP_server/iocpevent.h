#ifndef __IOCP_PORT_EVENT_H
#define __IOCP_PORT_EVENT_H

#include "messageBlock.h"

class IOCPEvent
{
public:
	virtual void handleExit(char *ip);
	virtual void handleRecv(char *buf, int len);
	virtual void handleSend(char *buf, int len);

	virtual void handleRecv(MessageBlock* msgBlock);
	virtual void handleRecv(IOCPMessageBlock* msgBlock);



};


#endif