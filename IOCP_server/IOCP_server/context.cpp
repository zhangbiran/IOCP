#include "context.h"
#include "socket.h"
#include "stream.h"


CContextBase::CContextBase(StreamSocket* streamSock) : _streamSock(streamSock)
{
}

CContextBase::~CContextBase() 
{
	if (_streamSock != NULL)
	{
		delete _streamSock;
		_streamSock = NULL;
	}
}


void CContextBase::onRecv(std::vector<StreamBuf>& streamSeq)
{

}

void CContextBase::send(char* buf, int len)
{
	_streamSock->send(buf, len);
}

CContext::~CContext()
{
}

CContext::CContext(StreamSocket* streamSock) : CContextBase(streamSock)
{

}

void CContext::onRecv(std::vector<StreamBuf>& streamSeq)
{

}