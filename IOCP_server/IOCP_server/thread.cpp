#include "thread.h"
#include <process.h>
#include "iocp.h"
#include "memorypool.h"
#include "macrodef.h"
#include "constdef.h"
Thread::Thread()
{}

Thread::~Thread()
{}

extern IOCP* g_iocp;
extern MemoryPool * g_memoryPool;


std::map<int, DWORD> Thread::_threadIdKeyMap;
std::map<int, vector<char*> > Thread::_memoryMap;
std::map<int, int> Thread::_testMap;
Event Thread::_tlsEvent;
Event Thread::_memoryEvent;
Event Thread::_testEvent;
Event Thread::_allIOCPThreadStartEvent(TRUE, FALSE);


bool Thread::createTSL()
{
	DWORD key = ::TlsAlloc();
	if (key == TLS_OUT_OF_INDEXES)
	{
		return false;
	}

	_tlsEvent.wait();
	_threadIdKeyMap[Thread::getThreadId()] = key;
	for (std::map<int, DWORD>::iterator it = _threadIdKeyMap.begin();
		it != _threadIdKeyMap.end(); ++it)
	{
		cout << it->first << " : " << it->second << endl;
	}
	cout << "------------------------------" << endl;
	_tlsEvent.signal();

	return true;
}

bool Thread::deleteTSL(DWORD key)
{
	return 0 != ::TlsFree(key);
}

LPVOID Thread::getValue(DWORD index)
{
	return ::TlsGetValue(index);
}

LPVOID Thread::getValue()
{
	DWORD index = _threadIdKeyMap[Thread::getThreadId()];
	std::map<int, DWORD>::iterator it= _threadIdKeyMap.find(getThreadId());
	if (it == _threadIdKeyMap.end())
	{
		return NULL;
	}

	return ::TlsGetValue(it->second);
}

bool Thread::setValue(DWORD index, LPVOID lpData)

{
	return 0 != ::TlsSetValue(index, lpData);
}


bool Thread::createBuffer()
{
	_memoryEvent.wait();
	vector<char*> _vec;
	for (int i = 0; i < CONST_THREAD_BUFFER_NUM; ++i)
	{
		char *p = (char*)::malloc(CONST_THREAD_BUFFER_SIZE);
		assert(NULL != p);
		_vec.push_back(p);
	}

	_memoryMap[getThreadId()] = _vec;
	_memoryEvent.signal();
	return true;
}

bool Thread::createBufferNoEvent(int num)
{
	int threadId = getThreadId();
	std::vector<char*>& vec = _memoryMap[threadId];
	for (int i = 0; i < CONST_THREAD_BUFFER_NUM; ++i)
	{
		char *p = (char*)::malloc(CONST_THREAD_BUFFER_SIZE);
		assert(NULL != p);

		vec.push_back(p);
	}
	
	return true;
}


bool Thread::testCreate()
{
	_testEvent.wait();
	_testMap[(int)getThreadId()] = (int)getThreadId();

	_testEvent.signal();

	return false;
}

char * Thread::getBuffer()
{
	if (_memoryMap.empty())
	{
		createBufferNoEvent();
	}
	assert(!_memoryMap.empty());
	std::vector<char*>&vec = _memoryMap[getThreadId()];
	char *p = vec.back();
	vec.pop_back();
	cout << "getBuffer size: " << vec.size() << endl;
	return p;
}

void Thread::addBuffer(char * const p)
{
	std::vector<char*>&vec = _memoryMap[getThreadId()];
	vec.push_back(p);
	if (vec.size() >= CONST_THREAD_BUFFER_NUM * 2)
	{
		int needDeleteNum = vec.size() - CONST_THREAD_BUFFER_NUM;
		for (int i = 0; i < needDeleteNum; ++i)
		{
			char *p = vec.back();
			delete p;
			vec.pop_back();
		}
		cout << "delete buffer" << endl;
	}
	cout << "addBuffer size: " << vec.size() << endl;
}

unsigned _stdcall Thread::threadProc(void *arg)
{

	//PRINT_TICK_COUNT(Thread::createTSL());
	//PRINT_TICK_COUNT_STR(Thread::createBuffer(), "create buffer");
	//Thread::testCreate();

	//_allIOCPThreadStartEvent.wait();

	g_iocp->getQueuedCompletionStatus();

	/*
	while (1)
	{
		DWORD tick = GetTickCount();
		//char *p = g_memoryPool->allocBuff();
		char *p = (char*)malloc(102400);
		DWORD tick2 = GetTickCount();
		int num = tick2 - tick;
		if (num > 10)
		{
			cout << "tick: " << num << endl;
		}
	
		memcpy(p, "hello", 5);
		*(p + 5) = 0;
		//cout << p << endl;
		//g_memoryPool->freeBuff(p);
	}
	*/


	
	while (1)
	{
		PRINT_TICK_COUNT_STR(_allIOCPThreadStartEvent.wait(), "_allIOCPThreadStartEvent");
	}
	while (1)
	{
		char *p = getBuffer();

		addBuffer(p);
	}

	cout << "all thread Started" << endl;

	return true;
}

bool Thread::start(void *arg)
{
	uintptr_t thread_handle = _beginthreadex(
		NULL,
		0,
		(unsigned(__stdcall *)(void*))Thread::threadProc,
		arg,
		0,
		NULL);

	return -1 != thread_handle;
}