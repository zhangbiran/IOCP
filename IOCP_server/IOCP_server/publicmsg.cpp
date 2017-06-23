#include "publicmsg.h"


void Message1::serialize(StreamBuf& streamBuf)
{
	streamBuf >> _ch;
	streamBuf >> _id;
	streamBuf >> _score;
	streamBuf >> _vec;
	streamBuf >> _map;
	streamBuf >> _str;
}

void Message1::deserialization(StreamBuf& streamBuf)
{
	streamBuf << _ch;
	streamBuf << _id;
	streamBuf << _score;
	streamBuf << _vec;
	streamBuf << _map;
	streamBuf << _str;
}