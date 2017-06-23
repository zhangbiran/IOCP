#ifndef __MSG_KEYWORD_MANAGER_H
#define __MSG_KEYWORD_MANAGER_H
#include "include.h"
class CkeyWord
{
public:
	CkeyWord(string name) :_keyWordName(name) {}
	~CkeyWord();
	virtual bool isSysKeyWord() { return true; }
	string _keyWordName;
};

class CCustomKeyWord : public CkeyWord
{
public:
	CCustomKeyWord(string key) : CkeyWord(key) {}
	bool isSysKeyWord() { return false; }
};

class CKeyWordManager
{
public:
	static CKeyWordManager* instance();
	bool isKeyWord(string name);
	void addKeyWord(CkeyWord* keyWord);
	CkeyWord * findKeyWord(string name);
private:
	CKeyWordManager();
	~CKeyWordManager();
	void init();

	std::map<string, CkeyWord*> _keyWordMap;
};

#endif