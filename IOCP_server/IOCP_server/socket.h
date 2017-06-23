#ifndef __SOCKET_IOCP_H
#define __SOCKET_IOCP_H
#include "include.h"
#include "stream.h"
#include "sockaddr.h"
#include "iocpdef.h"

class StreamSocket;
class SocketAddr;


class Socket
{
public:
	SOCKET getSocket() { return _socket; }
	void setSocket(SOCKET socket) { _socket = socket; }
	void closeSocket() { ::closesocket(_socket); _socket = SOCKET_ERROR; }
protected:
	Socket();
	~Socket();
	
	SOCKET _socket;
};

class AcceptSocket : public Socket
{
public:
	AcceptSocket(bool isBlock = true);
	~AcceptSocket();
	bool open(sockaddr_in& addr);
	bool open(SocketAddr& socketAddr);
	bool accept(StreamSocket& client);
	bool accept(StreamSocket* client);

private:
	bool open_i(sockaddr_in& addr);
	bool openBlock(sockaddr_in& addr);
	bool openNoBlock(sockaddr_in& addr);

	bool accept_i(StreamSocket& client);
	bool acceptBlock(StreamSocket& client);
	bool acceptNoBlock(StreamSocket& client);
	bool _isBlock;
};

class ConnectSocket : public Socket
{
public:
	ConnectSocket();
	~ConnectSocket();
	bool open(SocketAddr& serverAddr);
	bool connect(StreamSocket& server);

private:
	SocketAddr _serverAddr;
};

class StreamSocket : public Socket
{
public:
	friend class AcceptSocket;
	friend class ConnectSocket;
	StreamSocket();
	StreamSocket(SOCKET sock);
	~StreamSocket();

	//²Ù×÷
	bool recv(IOStream100& stream);
	bool recv(IO_DATA* ioData);
	bool recv();

	bool send(char *buf, int len);
	bool send(IO_DATA* perIOData);
	bool send(StreamBuf& streamBuf);


	bool resend(READ_IO_DATA* ioData);


	bool setSocketRecvBuffer(int size);
	bool setSocketSendBuffer(int size);
	void printSocketRecvBuffer();
	void printSocketSendBuffer();
	void printCanReadBufferSize();

	void print()
	{
		if (getSocket() != SOCKET_ERROR)
		{
			char *ip = inet_ntoa(_addr.sin_addr);
			cout << ip << endl;
		}

		int size = 65535;
		int len = sizeof(int);
		setsockopt(
			_socket,
			SOL_SOCKET,
			SO_RCVBUF,
			(char*)&size,
			len
			);

		setsockopt(
			_socket,
			SOL_SOCKET,
			SO_SNDBUF,
			(char*)&size,
			len
			);


		int recvbuffSize = 0;
		int optlen = sizeof(int);
		if (getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&recvbuffSize, &optlen) == 0)
		{
			cout << "recvbuffSize:" << recvbuffSize << endl;
		}

		int sendBuffSize = 0;
		if (getsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBuffSize, &optlen) == 0)
		{
			cout << "recvbuffSize:" << sendBuffSize << endl;
		}
	}
	sockaddr_in& getAddr() { return _addr; }


private:
	void init()
	{
		ZeroMemory(&_addr, sizeof(sockaddr_in));
	}

private:
	sockaddr_in _addr;
};

#endif