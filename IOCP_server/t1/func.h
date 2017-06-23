#ifndef __MSG_FUNC_H
#define __MSG_FUNC_H

#include "include.h"
#include "KeywordManager.h"
class Line;

class Param
{
public:
	string _paramType;
	string _paramName;
};

class Func
{
public:
	Func();
	bool analyze();
	bool writeH(string& str);
	bool writeCPP(string& str);

	string _funcName;
	string _funcPrototype;
	Line*  _line;
	
};

#endif