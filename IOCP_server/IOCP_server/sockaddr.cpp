#include "sockaddr.h"

SocketAddr::SocketAddr()
{
	ZeroMemory(&_addr, sizeof(sockaddr_in));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(8888);
	_addr.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");
	_len = sizeof(sockaddr_in);
}

SocketAddr::SocketAddr(int port, char *ip/* = "127.0.0.1"*/)
{
	ZeroMemory(&_addr, sizeof(sockaddr_in));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.S_un.S_addr = inet_addr(ip);
	_len = sizeof(sockaddr_in);
}

SocketAddr::~SocketAddr()
{

}