#include "iocp.h"
#include "publicmsg.h"
#include "stream.h"
#include "publicconfig.h"
#include "messageBlock.h"
#include "iocpdef.h"
#include "Exception.h"

IOCP::IOCP()
{
	_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	_iocpEvent = new IOCPEvent();
}

IOCP::~IOCP()
{

}

bool IOCP::add(HANDLE sock, ULONG_PTR key)
{
	HANDLE handle = CreateIoCompletionPort(sock, _handle, key, 0);
	return handle != NULL;
}

bool IOCP::add(StreamSocket& streamSocket, HANDLE_DATA *handleData)
{
	if (SOCKET_ERROR == streamSocket.getSocket())
	{
		return false;
	}

	return add((HANDLE)streamSocket.getSocket(), (ULONG_PTR)handleData);
}

bool IOCP::add(StreamSocket* streamSocket, HANDLE_DATA *handleData)
{
	if (SOCKET_ERROR == streamSocket->getSocket())
	{
		return false;
	}

	return add((HANDLE)streamSocket->getSocket(), (ULONG_PTR)handleData);
}

bool IOCP::add(AcceptSocket& accpetSocket, HANDLE_DATA *handleData)
{
	if (SOCKET_ERROR == accpetSocket.getSocket())
	{
		return false;
	}

	return add((HANDLE)accpetSocket.getSocket(), (ULONG_PTR)handleData);
}

bool IOCP::add(AcceptSocket* acceptSocket, HANDLE_DATA *handleData)
{
	if (SOCKET_ERROR == acceptSocket->getSocket())
	{
		return false;
	}

	return add((HANDLE)acceptSocket->getSocket(), (ULONG_PTR)handleData);
}

bool IOCP::getQueuedCompletionStatus()
{
	DWORD sizeTransfer;
	HANDLE_DATA * handle_data = NULL;
	IO_DATA * ioData = NULL;
	LPOVERLAPPED overlapped;
	while (true)
	{
		int ret = GetQueuedCompletionStatus(_handle, &sizeTransfer, (PULONG_PTR)&handle_data, (LPOVERLAPPED*)&overlapped, INFINITE);
		ioData = (IO_DATA*)CONTAINING_RECORD(overlapped, IO_DATA, overlapped);
		if (ret == 0)
		{
			_iocpEvent->handleExit(inet_ntoa(handle_data->getAddr().sin_addr));
			delete handle_data;
			delete ioData;
			continue;
		}

		try
		{
			ioData->handleEvent(handle_data, sizeTransfer);
		}
		catch (const exception& e)
		{
			std::cerr << e.what() << '\n';

			delete ioData;
			delete handle_data;
		}
		

	}
	return true;
}
