#ifndef __IOCP_PUBLIC_CONFIG_H
#define __IOCP_PUBLIC_CONFIG_H
#include "iocpdef.h"
class PublicConfig
{
public:
	void parseComandLine(int argc, char** argv)
	{
		if (argc >= 2)
		{
			int type = atoi(argv[1]);
			_type = (AppType)type;
		}

		if (isServer())
		{
			cout << "is server" << endl;
		}
		else if (isClient())
		{
			cout << "is client" << endl;
		}
	}
	bool isServer() { return AppType_Server == _type; }
	bool isClient() { return AppType_Client == _type; }

	static PublicConfig* intance()
	{
		static PublicConfig ins;
		return &ins;
	}
private:
	
	PublicConfig() {}
	~PublicConfig() {}
	
	AppType _type;
};

#endif