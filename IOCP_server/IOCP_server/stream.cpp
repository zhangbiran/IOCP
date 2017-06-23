#include "stream.h"
#include "include.h"

void StreamBuf::init()
{
	_buf = NULL;
	_curSize = 0;
	_totalSize = 0;
	_readSize = 0;
}


StreamBuf::StreamBuf()
{
	init();
	reSize(1024);
}

StreamBuf::StreamBuf(int size)
{
	init();
	reSize(size);
}

StreamBuf::StreamBuf(char *buf, int len)
{
	init();
	_buf = buf;
	_curSize = len;
	_totalSize = len;
	
}

StreamBuf::~StreamBuf()
{
	if (_buf != NULL)
	{
		free(_buf);
	}
	
}



void StreamBuf::reSize(int size)
{
	assert(size > 0);
	if (size <= _totalSize)
	{
		return;
	}
	else
	{
		if (_buf == NULL)
		{
			if (size < 1024)
			{
				size = 1024;
			}
			_buf = (char*)malloc(size);
			assert(_buf);
			adjust(size);
			return;
		}

		char *temp = (char*)realloc(_buf, _totalSize + 1024);
		assert(temp != NULL);
		if (temp != _buf)
		{
			memcpy(temp, _buf, _curSize);
			_buf = temp;
		}
		
		adjust(_totalSize + 1024);
	}
}

void StreamBuf::print()
{
	_buf[_curSize] = 0;
	cout << "_buf:" << _buf << endl;
	cout << "_curSize:" << _curSize << endl;
	cout << "_totalSize:" << _totalSize << endl;
	cout << "-------------------------------------" << endl;
}

void StreamBuf::addStr(const char *str, int len)
{
	if (_curSize + len > _totalSize)
	{
		reSize(_curSize + len);
	}

	memcpy(_buf + _curSize, str, len);
	_curSize += len;
}
void StreamBuf::subStr(int len)
{
	if (len > _curSize)
	{
		_curSize = 0;
	}
	else
	{
		_curSize -= len;
	}
}

void StreamBuf::adjust(int size)
{
	if (_totalSize < size)
	{
		_totalSize = size;
	}
}


StreamBuf& StreamBuf::operator >> (char data)
{
	reSize(_curSize + sizeof(char));
	*(_buf + _curSize) = data;
	_curSize += sizeof(char);
	return *this;
}

StreamBuf& StreamBuf::operator >> (int data)
{
	reSize(_curSize + sizeof(int));
	memcpy(_buf + _curSize, &data, sizeof(int));
	_curSize += sizeof(int);
	return *this;
}

StreamBuf& StreamBuf::operator >> (float data)
{
	reSize(_curSize + sizeof(int));
	memcpy(_buf + _curSize, &data, sizeof(float));
	_curSize += sizeof(float);
	return *this;
}

StreamBuf& StreamBuf::operator >> (std::vector<int> vecInt)
{
	int len = vecInt.size();
	reSize(_curSize + sizeof(int)* len);
	*this >> len;
	for (int i = 0; i < len; ++i)
	{
		*this >> vecInt[i];
	}

	return *this;
}

StreamBuf& StreamBuf::operator >> (std::map<int, int> mapIntInt)
{
	int len = mapIntInt.size();
	reSize(_curSize + sizeof(int));
	*this >> len;
	for (std::map<int, int>::iterator it = mapIntInt.begin();
		it != mapIntInt.end(); ++it)
	{
		*this >> it->first;
		*this >> it->second;
	}

	return *this;
}

StreamBuf& StreamBuf::operator >> (std::string str)
{
	int len = str.length();
	reSize(_curSize + sizeof(int));
	*this >> len;
	reSize(_curSize + len);
	memcpy(_buf + _curSize, str.c_str(), str.length());
	_curSize += len;

	return *this;
}

StreamBuf& StreamBuf::operator << (char& data)
{
	if (_readSize + sizeof(char) > _curSize)
	{
		data = -1;
		return *this;
	}

	data = *(_buf + _readSize);
	_readSize += sizeof(char);
	return *this;
}

StreamBuf& StreamBuf::operator << (int& data)
{
	if (_readSize + sizeof(int) > _curSize)
	{
		data = -1;
		return *this;
	}
	memcpy(&data, _buf + _readSize, sizeof(int));
	_readSize += sizeof(int);
	return *this;
}

StreamBuf& StreamBuf::operator << (float& data)
{
	if (_readSize + sizeof(float) > _curSize)
	{
		data = -1.0;
		return *this;
	}

	memcpy(&data, _buf + _readSize, sizeof(float));
	_readSize += sizeof(float);
	return *this;
}

StreamBuf& StreamBuf::operator << (std::vector<int>& vecInt)
{
	int len = 0;
	if (_readSize + sizeof(int) > _curSize)
	{
		return *this;
	}
	*this << len;
	int data;
	for (int i = 0; i < len; i++)
	{
		*this << data;
		vecInt.push_back(data);
	}

	return *this;
}

StreamBuf& StreamBuf::operator << (std::map<int, int>& mapIntInt)
{
	int len = 0;
	if (_readSize + sizeof(int) > _curSize)
	{
		return *this;
	}
	*this << len;
	int first = 0;
	int second = 0;
	for (int i = 0; i < len; ++i)
	{
		*this << first;
		*this << second;
		mapIntInt[first] = second;
	}

	return *this;
}

StreamBuf& StreamBuf::operator << (std::string& str)
{
	if (_readSize + sizeof(int) > _curSize)
	{
		return *this;
	}
	
	int len = 0;
	*this << len;
	if (_readSize + len > _curSize)
	{
		return *this;
	}
	string temp(_buf + _readSize, len);
	_readSize += len;
	str = temp;

	return *this;
}