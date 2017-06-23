#ifndef __IOCP_EVENT_H
#define __IOCP_EVENT_H
#include "include.h"

class Event
{
public:
	Event(BOOL isManulSet = FALSE, BOOL isInitSignaled = TRUE)
	{
		_event = ::CreateEvent(
			NULL,
			isManulSet,
			isInitSignaled,
			NULL);
	}
	~Event()
	{
		::CloseHandle(_event);
	}
	bool wait(DWORD _Milliseconds = INFINITE)
	{
		DWORD ret = ::WaitForSingleObject(_event, _Milliseconds);
		if (ret == WAIT_OBJECT_0)
		{
			return true;
		}
		else if (ret == WAIT_TIMEOUT)
		{
			return false;
		}
		else if (ret == WAIT_FAILED)
		{
			return false;
		}

		return false;
	}

	bool signal()
	{
		return ::SetEvent(_event) != 0;
	}

private:
	HANDLE _event;
};


#endif