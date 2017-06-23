#include <iostream>
using namespace std;

#include <windows.h>
#include <exception>


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

//exception
int main(void)
{
	try
	{
		CException e("hello");
		throw e;
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
	return 0;
}