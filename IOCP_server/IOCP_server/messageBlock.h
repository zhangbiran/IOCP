#ifndef __IOCP_MESSAGE_BLOCK_H
#define __IOCP_MESSAGE_BLOCK_H
#include "include.h"
#include "stream.h"
#include "constdef.h"
class MessageHead
{
public:
	int command;
	int len;
};

class MessageBlock
{
public:
	MessageBlock(int size = CONST_THREAD_BUFFER_SIZE);
	~MessageBlock();
	void conn(MessageBlock * nextMb);
	bool enoughBuffer(int size);
	int getLastBuffSize();
	char *getWriteP();
	const char *getReadP();
	int getUnReadSize();
	void addReadSize(int size);
	void addLen(int size);

	void freeBack();

	char *_buf;
	int _len;
	int _bufferLen;
	int _readLen;
	MessageBlock *_next;
};

class IOCPMessageBlock
{
public:
	IOCPMessageBlock();
	~IOCPMessageBlock();
	void onRecv(int transfered);
	void addTotalLen(int size);
	void subTotalLen(int size);
	void getStreamBufs(std::vector<StreamBuf*>& streamBufs);
	bool isFinished();
	void adjustBuffer();
	char* getWriteP();
	int getNextMsgSize();
	int getLastBuffSize();

	MessageBlock *_data;
	DWORD _totalLen;
	MessageBlock * _head;
	MessageBlock * _tail;
};

#endif