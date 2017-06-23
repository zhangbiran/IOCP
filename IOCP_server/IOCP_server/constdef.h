#ifndef __IOCP_CONST_H
#define __IOCP_CONST_H
#include "include.h"
const int CONST_THREAD_BUFFER_NUM = 100;	//每个线程Buffer的数量
const int CONST_THREAD_BUFFER_SIZE = 1024;	//线程Buffer的长度
const int CONST_THREAD_BUFFER_ADD_NUM = 100;//线程Buffer的数量空了每次加的数量
const int CONST_PER_READ_SIZE = 200;		//每次接收的字节
const int CONST_PER_SEND_SIZE = 200;		//每次发送的字节
const int CONST_MIN_PER_READ_SIZE = 100;	//最小允许接收字节



#endif