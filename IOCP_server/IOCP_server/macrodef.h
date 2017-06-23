#ifndef __IOCP_MACRO_H
#define __IOCP_MACRO_H
#include "include.h"
#include "Exception.h"
#define PRINT_TICK_COUNT(OPER)\
	{ \
		DWORD tick = GetTickCount(); \
		OPER; \
		DWORD tick2 = GetTickCount(); \
		cout <<"tick count = " << tick2 - tick << endl; \
	} \

#define PRINT_TICK_COUNT_STR(OPER, STR)\
	{ \
	DWORD tick = GetTickCount(); \
	OPER; \
	DWORD tick2 = GetTickCount(); \
	cout << STR << " tick count = " << tick2 - tick << endl; \
	} \


#define THROW_EXCEPTION(ERR_STR) \
	{\
	CException e(ERR_STR); \
	throw e; \
	} \

#endif