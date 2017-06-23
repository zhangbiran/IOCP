#ifndef __IOCP_PUBLIC_MSG_H
#define __IOCP_PUBLIC_MSG_H
#include "stream.h"
#include "include.h"
class Message1
{
public:
	Message1()
	{
		init();
	}
	void init()
	{
		_ch = 0;
		_id = 0;
		_score = 0;
	}
	void serialize(StreamBuf& streamBuf);
	void deserialization(StreamBuf& streamBuf);

	void print()
	{
		cout << "_ch: " << _ch << endl;
		cout << "_id: " << _id << endl;
		cout << "_score: " << _score << endl;
		for (int i = 0; i < _vec.size(); ++i)
		{
			cout << _vec[i] << " ";
		}
		cout << endl;

		for (std::map<int, int>::iterator it = _map.begin();
			it != _map.end(); ++it)
		{
			cout << it->first << ":" << it->second << endl;
		}

		cout << _str << endl;
	}

	char _ch;
	int _id;
	float _score;
	std::vector<int> _vec;
	std::map<int, int> _map;
	string _str;
};

class Message2
{
public:
	void serialize(StreamBuf& streamBuf);
	void deserialization(StreamBuf& streamBuf);

	Message1 _msg;

};

#endif