#ifndef __IOCP_STREAM_H
#define __IOCP_STREAM_H
#include <iostream>
using namespace std;
#include "include.h"


class IOStream100
{
public:
	int getBufSize() { return 100-1; }
	void print()
	{
		buf[len] = 0;
		cout << buf << endl;
	}
	char buf[100];
	int len;
};

class StreamBuf
{
public:
	StreamBuf();
	StreamBuf(int size);
	StreamBuf(char *buf, int len);
	~StreamBuf();
	
	void reSize(int size);
	void print();
	void addStr(const char *str, int len);
	void subStr(int len);
	StreamBuf& operator >> (char data);
	StreamBuf& operator >> (int data);
	StreamBuf& operator >> (float data);
	StreamBuf& operator >> (std::vector<int> vecInt);
	StreamBuf& operator >> (std::map<int, int> mapIntInt);
	StreamBuf& operator >> (std::string str);

	StreamBuf& operator << (char& data);
	StreamBuf& operator << (int& data);
	StreamBuf& operator << (float& data);
	StreamBuf& operator << (std::vector<int>& vecInt);
	StreamBuf& operator << (std::map<int, int>& mapIntInt);
	StreamBuf& operator << (std::string& str);

	char *getBuf() { return _buf; };
	int length() { return _curSize; }

private:
	void init();
	void adjust(int size);
	char *_buf;
	int _curSize;
	int _totalSize;
	int _readSize;
};

#endif