#ifndef __IOCP_EXCEPTION_H
#define __IOCP_EXCEPTION_H


#include "include.h"
class CException
	:public std::exception
{
public:

	CException(const char* msg = "CException", int code = 0)
		:_str(msg), _code(code)
	{
	}

	CException(const CException& ex)
		:_str(ex._str), _code(ex._code)
	{
	}

	CException& operator = (const CException& ex)
	{
		_str = ex._str;
		_code = ex._code;
		return *this;
	}

	const char* what() const throw()
	{
		return _str.c_str();
	}

	int code() const
	{
		return _code;
	}

	void setMessage(const std::string& msg)
	{
		_str = msg;
	}

	void setCode(int code)
	{
		_code = code;
	}
protected:
	std::string _str;
	int _code;  //the exeption code
};

#endif