#include "include.h"
#include "iocp.h"
#include "sockaddr.h"
#include "thread.h"

#pragma comment(lib, "ws2_32.lib")

extern IOCP* g_iocp = new IOCP();

int main(int argc, char** argv)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		cout << "socket error" << endl;
	}
	/*
	HOSTENT *localhost = gethostbyname("");
	char *ip = inet_ntoa(*(struct in_addr*)*localhost->h_addr_list);
	cout << ip << endl;
	sockaddr_in server_addr = { 0 };
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8888);
	server_addr.sin_addr.s_addr = inet_addr("192.168.92.76");
	*/

	SocketAddr serverAddr(8888, "127.0.0.1");
	ConnectSocket connector;
	connector.open(serverAddr);
	StreamSocket server;
	if (!connector.connect(server))
	{
		cout << "connect failed" << endl;
		return 0;
	}
	cout << "connected" << endl;

	HANDLE_DATA *handle_data = new HANDLE_DATA(server.getSocket(),server.getAddr());
	if (!g_iocp->add(server, handle_data))
	{
		cout << "add failed" << endl;
		return 0;
	}
	
	Thread thread;
	thread.start(NULL);
	
	
	server.recv();

	char buf[100];
	while (cin.getline(buf+4, 100 - 1))
	{
		cout << "...." << endl;
		int *len1 = (int*)buf;
		int msgSize = cin.gcount() - 1;
		*len1 = msgSize;// CONST_PER_SEND_SIZE + 1;
		cout << *len1 << endl;
		server.send(buf, msgSize + 4);

		static int time = 1;
		if (time >= 3)
		{
			//closesocket(sock);
		}
		++time;
	
		u_long len = 0;
		if (ioctlsocket(sock, FIONREAD, &len) == 0)
		{
			cout << "can read in buffer: " << len << endl;
		}
		

	}
	return 0;
}