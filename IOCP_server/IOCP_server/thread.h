#ifndef __IOCP_THREAD_H
#define __IOCP_THREAD_H

#include "include.h"
#include "Event.h"
#include "constdef.h"

class Thread
{
public:
	Thread();
	~Thread();
	//Local Storage
	static bool createTSL();
	static bool deleteTSL(DWORD key);
	static LPVOID getValue(DWORD index);
	static LPVOID getValue();
	static bool setValue(DWORD index, LPVOID lpData);

	static bool createBuffer();
	static bool createBufferNoEvent(int num = CONST_THREAD_BUFFER_ADD_NUM);
	static bool testCreate();

	static char *getBuffer();
	static void addBuffer(char * const p);
	//
	static DWORD getThreadId() { return ::GetCurrentThreadId(); }

	static unsigned _stdcall threadProc(void *arg);
	bool start(void *arg);

	static void noticeAllThreadStart()
	{
		_allIOCPThreadStartEvent.signal();
	}
private:
	uintptr_t _handle;
	static std::map<int, DWORD> _threadIdKeyMap;
	static std::map<int, vector<char*> > _memoryMap;
	static std::map<int, int> _testMap;
	static Event _tlsEvent;
	static Event _memoryEvent;
	static Event _testEvent;
	static Event _allIOCPThreadStartEvent;
};



#endif