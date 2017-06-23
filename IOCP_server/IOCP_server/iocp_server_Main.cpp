#include <iostream>
using namespace std;
#include "include.h"
#include "socket.h"
#include "thread.h"
#include "iocp.h"
#include "stream.h"
#include "publicmsg.h"
#include "publicconfig.h"
#include "memorypool.h"
#include "userobject.h"


extern IOCP* g_iocp = new IOCP();
extern MemoryPool * g_memoryPool = new MemoryPool();

#pragma comment(lib, "ws2_32.lib")
int main(int argc, char** argv)
{
	PublicConfig::intance()->parseComandLine(argc, argv);
	/*
	StreamBuf sbuf(1);
	sbuf.addStr("helloworld", strlen("helloworld"));
	sbuf.print();
	sbuf.reSize(10);
	sbuf.addStr("nimei", 5);
	sbuf.print();
	sbuf.subStr(3);
	sbuf.reSize(2000);
	sbuf.print();
	*/

	Message1 msg1, msg2;
	msg1._ch = 'A';
	msg1._id = 10086;
	msg1._score = 99.9f;
	msg1._vec.push_back(1);
	msg1._vec.push_back(3);
	msg1._vec.push_back(5);
	msg1._vec.push_back(7);
	msg1._vec.push_back(9);
	msg1._vec.push_back(100);
	msg1._vec.push_back(200);
	msg1._map[1] = 100;
	msg1._map[2] = 100;
	msg1._map[3] = 100;
	msg1._map[4] = 100;
	msg1._map[5] = 100;
	msg1._str = "hello wolrd";

	StreamBuf sbuf1;
	msg1.serialize(sbuf1);
	msg2.deserialization(sbuf1);
	msg2.print();
	sbuf1.print();


	g_iocp = new IOCP();

	for (int i = 0; i < 10; i++)
	{
		Thread thread;
		thread.start(NULL);
	}

	Thread::noticeAllThreadStart();

	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SocketAddr socketAddr;
	AcceptSocket acceptor;
	if (!acceptor.open(socketAddr))
	{
		cout << "failed" << endl;
	}
	
	while (1)
	{
		
		//StreamSocket client;
		StreamSocket * client = new StreamSocket();
		if (acceptor.accept(client))
		{
			HANDLE_DATA *handle_data = new HANDLE_DATA(client->getSocket(), client->getAddr());

			if (!g_iocp->add(client, handle_data))
			{
				cout << "add failed" << endl;
			}

			client->printCanReadBufferSize();
			client->printSocketSendBuffer();

			CUserObject * userObject = new CUserObject();
			CContext* context = new CContext(client);
			userObject->setContext(context);

			if (!client->recv())
			{
				delete handle_data;
			}
		}
	}
	
	WSACleanup();
	return 0;
}