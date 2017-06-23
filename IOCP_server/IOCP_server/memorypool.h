#ifndef __IOCP_MEMORY_POOL_H
#define __IOCP_MEMORY_POOL_H
#include "include.h"
#include "Event.h"

class MemoryPool
{
public:
	MemoryPool(int num = 1024);
	~MemoryPool();

	char * allocBuff();
	void freeBuff(char*p);
	void addBuffNum(int num = 1024 *4);
private:
	std::vector<char*> _memoryPool;


	Event _poolEvent;

};




#endif