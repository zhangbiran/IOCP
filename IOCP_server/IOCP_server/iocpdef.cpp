#include "iocpdef.h"

#include "socket.h"
#include "Exception.h"

HANDLE_DATA::HANDLE_DATA(SOCKET sock, sockaddr_in addr)
:_sock(sock), _addr(addr)
{
}

HANDLE_DATA::~HANDLE_DATA()
{
	closesocket(_sock);
}


READ_IO_DATA::~READ_IO_DATA()
{
}

bool READ_IO_DATA::handleEvent(HANDLE_DATA * handle_data, int transfered)
{
	_msgBlock._tail->_len += transfered;
	_msgBlock._totalLen += transfered;
	return true;
}

void READ_IO_DATA::makeWSABuf()
{
	_msgBlock.adjustBuffer();
	_wsaBuf.buf = _msgBlock.getWriteP();


	int len = _msgBlock.getLastBuffSize();
	if (len > CONST_PER_READ_SIZE)
	{
		len = CONST_PER_READ_SIZE;
	}

	_wsaBuf.len = len;
}

bool READ_IO_DATA::isFinished()
{
	return _msgBlock.isFinished();
}

void READ_IO_DATA::onFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData)
{
	std::vector<StreamBuf*> _streamBufSeq;
	_msgBlock.getStreamBufs(_streamBufSeq);

	for (int i = 0; i < _streamBufSeq.size(); ++i)
	{
		_streamBufSeq[i]->print();
	}
	
	getchar();
	StreamSocket streamSock(handle_data->getSock());
	if (!streamSock.recv(perIOData))
	{
		CException e("client has exit, recv error");
		throw e;
	}
}

void READ_IO_DATA::onNoFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData)
{
	StreamSocket streamSock(handle_data->getSock());

	if (!streamSock.recv(perIOData))
	{
		CException e("client has exit, send error");
		throw e;
	}
}

bool SEND_IO_DATA::handleEvent(HANDLE_DATA * handle_data, int transfered)
{
	cout << "has send: " << transfered << endl;
	_hasTransfer += transfered;
	return true;
}

bool SEND_IO_DATA::makeCopy(char *buf, int len)
{
	_buf = (char*)malloc(len);
	memcpy(_buf, buf, len);
	_len = len;

	return true;
}

void SEND_IO_DATA::makeWSABuf()
{
	_wsaBuf.buf = _buf + _hasTransfer;
	int lastLen = _len - _hasTransfer;
	if (lastLen >= CONST_PER_SEND_SIZE)
	{
		_wsaBuf.len = CONST_PER_SEND_SIZE;
	}
	else
	{
		_wsaBuf.len = lastLen;
	}
}

bool SEND_IO_DATA::isFinished()
{
	if (_hasTransfer >= _len)
	{
		return true;
	}

	return false;
}

void SEND_IO_DATA::onFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData)
{
	cout << "has send all:" << _hasTransfer << endl;
	delete perIOData;
}

void SEND_IO_DATA::onNoFinished(HANDLE_DATA * handle_data, IO_DATA* perIOData)
{
	StreamSocket streamSock(handle_data->getSock());
	if (!streamSock.send(perIOData))
	{
		delete perIOData;
		delete handle_data;
	}
}


IO_DATA::IO_DATA() : _io(NULL)
{
	assert((char*)this == (char*)&overlapped);
	zeroOverlapped();
}

IO_DATA::~IO_DATA()
{
	if (_io != NULL)
	{
		delete _io;
	}
}

void IO_DATA::zeroOverlapped()
{
	ZeroMemory(&overlapped, sizeof(OVERLAPPED));
}

bool IO_DATA::handleEvent(HANDLE_DATA * handle_data, int transfered)
{

	_io->handleEvent(handle_data, transfered);
	if (_io->isFinished())
	{
		_io->onFinished(handle_data, this);
	}
	else
	{
		_io->onNoFinished(handle_data, this);
	}


	return true;
}


