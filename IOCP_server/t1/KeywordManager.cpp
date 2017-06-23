#include "KeywordManager.h"


CkeyWord::~CkeyWord()
{

}

CKeyWordManager::CKeyWordManager()
{
	init();
}

CKeyWordManager::~CKeyWordManager()
{

}


CKeyWordManager* CKeyWordManager::instance()
{
	static CKeyWordManager ins;
	return &ins;
}

bool CKeyWordManager::isKeyWord(string name)
{
	if (_keyWordMap.find(name) == _keyWordMap.end())
	{
		return false;
	}

	return true;
}
void CKeyWordManager::addKeyWord(CkeyWord* keyWord)
{
	_keyWordMap[keyWord->_keyWordName] = keyWord;
}

CkeyWord * CKeyWordManager::findKeyWord(string name)
{
	std::map<string, CkeyWord*>::iterator it = _keyWordMap.find(name);
	if (it != _keyWordMap.end())
	{
		return it->second;
	}

}

void CKeyWordManager::init()
{
	_keyWordMap["char"] = new CkeyWord("char");
	_keyWordMap["bool"] = new CkeyWord("bool");
	_keyWordMap["int"] = new CkeyWord("int");
	_keyWordMap["float"] = new CkeyWord("float");
	_keyWordMap["long"] = new CkeyWord("long");

}