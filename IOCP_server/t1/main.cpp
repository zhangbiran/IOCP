#include <iostream>
using namespace std;
#include <fstream>
#include "file.h"
#include "include.h"
#include "Exception.h"

bool isBigDuan()
{
	int data = 0x01020304;
	char *p = (char*)&data;

	return *p == 0x01;
}

int main(void)
{
	{
		MsgFile file;
		try
		{
			file.open("msg.cdl");
			file.readAllLine();
			file.printAllLine();
			file.organization();
			file.analyze();
		}
		catch (const exception& e)
		{
			file.close();
			cout << e.what() << endl;
			return -1;
		}

		file.close();
		file.writeH();
		file.writeCPP();
	}
	{
	MsgFile file;
	//file.open("msg.txt", ios_base::in | ios_base::out | ios_base::trunc);
	file.readAllLine();
	file.printAllLine();
	file.close();

}

	return 0;
}