#include "memorypool.h"


MemoryPool::MemoryPool(int num)
{
	addBuffNum(num);
}
MemoryPool::~MemoryPool()
{
	for (int i = 0; i < _memoryPool.size(); ++i)
	{
		delete _memoryPool[i];
	}
}

void MemoryPool::addBuffNum(int num)
{
	for (int i = 0; i < num; ++i)
	{
		char *p = new char[1024];
		assert(p != NULL);
		_memoryPool.push_back(p);
	}
}

char* MemoryPool::allocBuff()
{
	char *p = NULL;
	_poolEvent.wait();

	if (_memoryPool.empty())
	{
		addBuffNum();
		
	}

	p = _memoryPool.back();
	_memoryPool.pop_back();
	cout << _memoryPool.size() << endl;
	_poolEvent.signal();
	return p;
}

void  MemoryPool::freeBuff(char*p)
{
	_poolEvent.wait();
	_memoryPool.push_back(p);
	cout << _memoryPool.size() << endl;
	_poolEvent.signal();
}