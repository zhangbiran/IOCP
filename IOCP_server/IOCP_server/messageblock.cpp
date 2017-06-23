
#include "include.h"
#include "messageBlock.h"
#include "constdef.h"
#include "Exception.h"

MessageBlock::MessageBlock(int size)
{
	_buf = (char*)malloc(CONST_THREAD_BUFFER_SIZE);
	_len = 0;
	_bufferLen = CONST_THREAD_BUFFER_SIZE;
	_readLen = 0;
	_next = NULL;
}

MessageBlock::~MessageBlock()
{
	if (_buf != NULL)
	{
		delete _buf;
	}
}

void MessageBlock::conn(MessageBlock * nextMb)
{
	if (_next != NULL)
	{
		_next->freeBack();
	}

	_next = nextMb;
}

void MessageBlock::freeBack()
{
	if (NULL == _next)
	{
		return;
	}

	MessageBlock * p = _next;
	MessageBlock * p2 = p->_next;
	while (p)
	{
		delete p;
		if (NULL == p2)
		{
			break;
		}

		p = p2;
		p2 = p->_next;
	}

	delete this;
}


bool MessageBlock::enoughBuffer(int size)
{
	if (_len + size > _bufferLen)
	{
		return false;
	}

	return true;
}

int MessageBlock::getLastBuffSize()
{
	return _bufferLen - _len;
}

char * MessageBlock::getWriteP()
{
	return _buf + _len;
}

const char *MessageBlock::getReadP()
{
	return _buf + _readLen;
}

int MessageBlock::getUnReadSize()
{
	return _len - _readLen;
}

void MessageBlock::addReadSize(int size)
{
	if (_readLen + size >= _len)
	{
		_readLen = _len;
	}
	else
	{
		_readLen += size;
	}
}

void MessageBlock::addLen(int size)
{
	_len += size;
}


IOCPMessageBlock::IOCPMessageBlock()
{
	_data = new MessageBlock();
	_totalLen = 0;
	_head = _data;
	_tail = _head;
}

IOCPMessageBlock::~IOCPMessageBlock()
{
	while (_head != NULL)
	{
		MessageBlock* temp = _head;
		_head = _head->_next;
		delete temp;
	}
}

void IOCPMessageBlock::onRecv(int transfered)
{
	_tail->addLen(transfered);
	addTotalLen(transfered);
}

void IOCPMessageBlock::addTotalLen(int size)
{
	_totalLen += size;
}

void IOCPMessageBlock::subTotalLen(int size)
{
	_totalLen -= size;
}

void IOCPMessageBlock::getStreamBufs(std::vector<StreamBuf*>& streamBufs)
{
	streamBufs.clear();
	MessageBlock *pHead = _head;
	while (pHead != NULL)
	{
		if (!isFinished())
		{
			break;
		}

		int needReadSize = getNextMsgSize();
		if (needReadSize == -1)
		{
			break;
		}
		
		int unReadSize = pHead->getUnReadSize();
		if (unReadSize >= 4)
		{
			pHead->addReadSize(4);
			
		}
		else
		{
			pHead->addReadSize(unReadSize);
			pHead = pHead->_next;
			pHead->addReadSize(4 - unReadSize);
		}

		StreamBuf * buf = new StreamBuf();
		int tempSize = needReadSize;
		while (tempSize > 0)
		{
			int unRead = pHead->getUnReadSize();
			if (unRead >= tempSize)
			{
				buf->addStr(pHead->getReadP(), tempSize);
				pHead->addReadSize(tempSize);
				break;
			}
			else
			{
				if (unRead > 0)
				{
					buf->addStr(pHead->getReadP(), unRead);
					pHead->addReadSize(unRead);
					tempSize -= unRead;
				}
				else
				{
					pHead = pHead->_next;
				}
				
			}
			
		}

		subTotalLen(4);
		subTotalLen(needReadSize);
		streamBufs.push_back(buf);
	}

	while (_head != NULL)
	{
		if (_head != pHead)
		{
			MessageBlock * temp = _head;
			_head = _head->_next;
			delete temp;
		}
		else
		{
			break;
		}
	}
}

char* IOCPMessageBlock::getWriteP()
{
	return _tail->getWriteP();
}

int IOCPMessageBlock::getNextMsgSize()
{
	if (_totalLen < 4)
	{
		return -1;
	}

	int data = 0;
	char *p = (char*)&data;
	int len = 4;
	MessageBlock* pHead = _head;
	while (pHead != NULL)
	{
		int unReadSize = pHead->getUnReadSize();
		if (unReadSize >= len)
		{
			memcpy(p, pHead->getReadP(), len);
			break;
		}
		else
		{
			if (unReadSize > 0)
			{
				memcpy(p, pHead->getReadP(), unReadSize);
				len -= unReadSize;
				p += unReadSize;
			}
		}

		pHead = pHead->_next;
	}

	if (data < 0 || data > CONST_PER_SEND_SIZE)
	{
		CException e("exception: needRead > CONST_PER_SEND_SIZE");
		throw e;
	}

	return data;
}

int IOCPMessageBlock::getLastBuffSize()
{
	return _tail->getLastBuffSize();
}

bool IOCPMessageBlock::isFinished()
{
	if (_totalLen < 4)
	{
		return false;
	}

	int temp = _totalLen - 4;
	int needSize = getNextMsgSize();
	if (needSize < 0)
	{
		return false;
	}
	
	bool isFinish = temp >= needSize;

	return isFinish;
}

void IOCPMessageBlock::adjustBuffer()
{
	if (!_tail->enoughBuffer(CONST_MIN_PER_READ_SIZE))
	{
		MessageBlock * mb = new MessageBlock();
		_tail->conn(mb);
		_tail = mb;
	}
}


