#include "socket.h"
#include "thread.h"
#include "iocpdef.h"



Socket::Socket() : _socket(SOCKET_ERROR)
{}
Socket::~Socket()
{

}

AcceptSocket::AcceptSocket(bool isBlock)
{
	_isBlock = isBlock;
}

AcceptSocket::~AcceptSocket()
{

}

bool AcceptSocket::open(sockaddr_in& addr)
{
	return open_i(addr);
}

bool AcceptSocket::open(SocketAddr& socketAddr)
{
	return open_i(socketAddr.getAddr());
}

bool AcceptSocket::accept(StreamSocket& client)
{
	return accept_i(client);
}

bool AcceptSocket::accept(StreamSocket* client)
{
	return accept(*client);
}



bool AcceptSocket::open_i(sockaddr_in& addr)
{
	if (_isBlock)
	{
		return openBlock(addr);
	}
	else
	{
		return openNoBlock(addr);
	}
}

bool AcceptSocket::openBlock(sockaddr_in& addr)
{
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sock)
	{
		return false;
	}

	int ret = ::bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == ret)
	{
		return false;
	}

	int maxConn = SOMAXCONN;
	ret = ::listen(sock, SOMAXCONN);
	if (SOCKET_ERROR == ret)
	{
		return false;
	}

	_socket = sock;

	return true;
}

bool AcceptSocket::openNoBlock(sockaddr_in& addr)
{
	if (!openBlock(addr))
	{
		return false;
	}

	u_long arp = 1;
	if (::ioctlsocket(_socket, FIONBIO, &arp) != 0)
	{
		::closesocket(_socket);
		_socket = SOCKET_ERROR;
		return false;
	}

	return true;
}

bool AcceptSocket::accept_i(StreamSocket& client)
{
	if (_isBlock)
	{
		return acceptBlock(client);
	}
	else
	{
		return acceptNoBlock(client);
	}
}

bool AcceptSocket::acceptBlock(StreamSocket& client)
{
	if (SOCKET_ERROR == _socket)
	{
		return false;
	}

	cout << "accept ..." << endl;

	int len = sizeof(sockaddr_in);
	SOCKET sock = ::accept(_socket, (struct sockaddr*)&client._addr, &len);
	if (SOCKET_ERROR == sock)
	{
		return false;
	}

	client.setSocket(sock);
	cout << "accepted: ";
	client.print();

	return true;
}

bool AcceptSocket::acceptNoBlock(StreamSocket& client)
{
	/*
	LPFN_ACCEPTEX     m_lpfnAcceptEx;         // AcceptEx函数指针  
	GUID GuidAcceptEx = WSAID_ACCEPTEX;        // GUID，这个是识别AcceptEx函数必须的  
	DWORD dwBytes = 0;

	WSAIoctl(
		_socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL);

	DWORD len;
	OVERLAPPED overlapped;
	m_lpfnAcceptEx(_socket, client.getSocket(), NULL, 0, 0, 0, &len, NULL);
	*/
	return true;
}


ConnectSocket::ConnectSocket()
{

}

ConnectSocket::~ConnectSocket()
{

}

bool ConnectSocket::open(SocketAddr& serverAddr)
{
	if (_socket != SOCKET_ERROR)
	{
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		cout << "socket error" << endl;
		return false;
	}

	_serverAddr = serverAddr;
	_socket = sock;

	return true;
}

bool ConnectSocket::connect(StreamSocket& server)
{
	if (SOCKET_ERROR == ::connect(_socket, (sockaddr*)&_serverAddr.getAddr(), _serverAddr.getAddrLen()))
	{
		cout << "connect failed" << endl;
		return false;
	}

	u_long arp = 1;
	if (::ioctlsocket(_socket, FIONBIO, &arp) != 0)
	{
		::closesocket(_socket);
		_socket = SOCKET_ERROR;
		return false;
	}

	server._socket = _socket;
	server._addr = _serverAddr.getAddr();
	
	return true;
}

StreamSocket::StreamSocket()
{
	init();
}


StreamSocket::StreamSocket(SOCKET sock)
{
	init();
	_socket = sock;
}

StreamSocket::~StreamSocket()
{

}

bool StreamSocket::recv(IOStream100& stream)
{
	int len = ::recv(_socket, stream.buf, stream.getBufSize(), 0);
	if (SOCKET_ERROR == len)
	{
		cout << "error" << endl;
		closesocket(_socket);
		_socket = SOCKET_ERROR;
		return false;
	}
	else if (0 == len)
	{
		cout << "close gracefully" << endl;
		closesocket(_socket);
		_socket = SOCKET_ERROR;
		return false;
	}
	else if (len > 0)
	{
		stream.len = len;
		return true;
	}
	else
	{
		_socket = SOCKET_ERROR;
		return false;
	}
}


bool StreamSocket::recv(IO_DATA* perIOData)
{	
	perIOData->zeroOverlapped();
	perIOData->_io->makeWSABuf();

	DWORD flags = 0;
	DWORD recvSize;
	int ret = WSARecv(_socket, &(perIOData->_io->_wsaBuf), 1, &recvSize, &flags, &(perIOData->overlapped), NULL);
	//cout << "&(pData->overlapped) : " << &(pData->overlapped) << endl;
	if (ret != 0)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}

		cout << "WSARecv error" << endl;
		return false;
	}
	
	return true;
}


bool StreamSocket::recv()
{
	IO_DATA * perIOData = new IO_DATA();
	READ_IO_DATA* ioData = new READ_IO_DATA();
	//IO_DATA *perIOData = (IO_DATA*)GlobalAlloc(GPTR, sizeof(IO_DATA));
	//ZeroMemory(perIOData, sizeof(IO_DATA));
	perIOData->_io = ioData;
	ioData->makeWSABuf();

	DWORD flags = 0;
	DWORD recvSize;
	int ret = WSARecv(_socket, &(ioData->_wsaBuf), 1, &recvSize, &flags, &(perIOData->overlapped), NULL);
	//cout << "&(pData->overlapped) : " << &(pData->overlapped) << endl;
	if (ret != 0)
	{
		int err = WSAGetLastError();
		if (err == WSA_IO_PENDING)
		{
			return true;
		}

		delete perIOData;
		cout << "WSARecv error: " << err << endl;
		return false;
	}
	
	return true;
}

bool StreamSocket::send(char *buf, int len)
{
	IO_DATA * perIOData = new IO_DATA();
	SEND_IO_DATA* ioData = new SEND_IO_DATA();
	perIOData->_io = ioData;
	
	ioData->makeCopy(buf, len);
	ioData->makeWSABuf();

	DWORD flags = 0;
	DWORD sendSize;
	int ret = WSASend(_socket, &(ioData->_wsaBuf), 1, &sendSize, flags, &(perIOData->overlapped), NULL);
	if (ret != 0)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}
		
		cout << "WSASend error" << endl;
		return false;
	}
	
	return true;

}

bool StreamSocket::resend(READ_IO_DATA* ioData)
{

	return true;
}

bool StreamSocket::send(StreamBuf& streamBuf)
{
	return send(streamBuf.getBuf(), streamBuf.length());
}


bool StreamSocket::send(IO_DATA* perIOData)
{
	perIOData->zeroOverlapped();
	perIOData->_io->makeWSABuf();

	DWORD flags = 0;
	DWORD sendSize;
	int ret = WSASend(_socket, &(perIOData->_io->_wsaBuf), 1, &sendSize, flags, &(perIOData->overlapped), NULL);
	if (ret != 0)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			return true;
		}

		cout << "WSASend error" << endl;
		closesocket(_socket);
		return false;
	}

	return true;
}



bool StreamSocket::setSocketRecvBuffer(int size)
{
	if (size < 0)
	{
		size = 65535;
	}
	int len = sizeof(int);
	int ret = setsockopt(
				_socket,
				SOL_SOCKET,
				SO_RCVBUF,
				(char*)&size,
				len
				);

	
	return ret == 0;
}

bool StreamSocket::setSocketSendBuffer(int size)
{
	if (size < 0)
	{
		size = 65535;
	}

	int len = sizeof(int);
	
	int ret = setsockopt(
				_socket,
				SOL_SOCKET,
				SO_SNDBUF,
				(char*)&size,
				len
				);

	return ret == 0;
}

void StreamSocket::printSocketRecvBuffer()
{
	int recvbuffSize = 0;
	int optlen = sizeof(int);
	if (getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&recvbuffSize, &optlen) == 0)
	{
		cout << "recvbuffSize:" << recvbuffSize << endl;
	}
}

void StreamSocket::printSocketSendBuffer()
{
	int sendBuffSize = 0;
	int optlen = sizeof(int);
	if (getsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBuffSize, &optlen) == 0)
	{
		cout << "recvbuffSize:" << sendBuffSize << endl;
	}
}

void StreamSocket::printCanReadBufferSize()
{
	u_long len = 0;
	if (ioctlsocket(_socket, FIONREAD, &len) == 0)
	{
		cout << "can read buffer size:" << len << endl;
	}
}