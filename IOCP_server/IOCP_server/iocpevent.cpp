#include "iocpevent.h"
#include "include.h"
#include "stream.h"

void IOCPEvent::handleExit(char *ip)
{
	cout << "remote exit:" << ip << endl;
}

void IOCPEvent::handleRecv(char *buf, int len)
{
	cout << "handleRecv buf: " << buf << " len: " << len << endl;
}

void IOCPEvent::handleSend(char *buf, int len)
{
	cout << "handleSend buf: " << buf << " len: " << len << endl;
}

void IOCPEvent::handleRecv(MessageBlock* msgBlock)
{
	msgBlock->_buf[msgBlock->_len] = 0;
	cout << "recv: " << msgBlock->_buf << endl;
}


void IOCPEvent::handleRecv(IOCPMessageBlock* iocpMb)
{
	for (MessageBlock* head = iocpMb->_head; head != NULL; head = head->_next)
	{
		head->_buf[head->_len] = 0;
		cout << head->_buf << endl;
	}
	cout << "--------------------" << endl;
}
