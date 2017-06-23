#include "ClassManager.h"

ClassManager* ClassManager::instance()
{
	static ClassManager ins;
	return &ins;
}



ClassManager::ClassManager()
{

}

ClassManager::~ClassManager()
{
	
}

bool ClassManager::isExistClassName(string& className)
{
	if (_classNameSet.count(className) > 0)
	{
		return true;
	}

	return false;
}
void ClassManager::addClassName(string& className)
{
	_classNameSet.insert(className);
}