#include "userobject.h"


CUserObject::CUserObject() : _context(NULL)
{

}

CContext* CUserObject::getContext()
{
	return _context;
}

void CUserObject::setContext(CContext* context)
{
	if (_context != NULL)
	{
		delete _context;
	}

	_context = context;
}