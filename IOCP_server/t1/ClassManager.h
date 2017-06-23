#ifndef __MSG_CLASS_MANAGER_H
#define __MSG_CLASS_MANAGER_H
#include "include.h"
class ClassManager
{
public:
	static ClassManager* instance();
	bool isExistClassName(string& className);
	void addClassName(string& className);

private:
	ClassManager();
	~ClassManager();

	std::set<string> _classNameSet;
};

#endif