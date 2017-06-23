#ifndef __IOCP_SOCKADDR_H
#define __IOCP_SOCKADDR_H
#include "include.h"
class SocketAddr
{
public:
	SocketAddr();
	SocketAddr(int port, char *ip = "127.0.0.1");
	~SocketAddr();
	sockaddr_in& getAddr() { return _addr; }
	int& getAddrLen() { return _len; }

private:
	sockaddr_in _addr;
	int _len;
};

#endif