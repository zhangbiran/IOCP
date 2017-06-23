#ifndef __IOCP_USER_OBJECT_H
#define __IOCP_USER_OBJECT_H
#include "include.h"
#include "context.h"

class CUserObject
{
public:
	CUserObject();
	CContext* getContext();
	void setContext(CContext* context);
private:
	CContext * _context;
};

#endif