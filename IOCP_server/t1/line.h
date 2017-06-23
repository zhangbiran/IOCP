#ifndef __MSG_LINE_H
#define __MSG_LINE_H

#include "include.h"
enum LineType
{
	LineTypeInclude = 0, //°üº¬
	LineTypeBreak = 1,	//¿ÕÐÐ
	LineTypeExplain = 2, //×¢ÊÍ	
	LineTypeClass = 3,	//Àà
	LineTypeFunc = 4,	//º¯Êý
	LineTypeAccess = 5,	//·ÃÎÊÐÞÊÎ·û
	LineTypeLeftBrace = 6, //×ó»¨À¨ºÅ
	LineTypeRightBrace = 7, //ÓÒ»¨À¨ºÅ
};


class Line
{
public:
	Line();
	bool read(int lineNum, fstream& f);
	void printLine();
	void split(char *tokens);
	int getType();
	void printError();

	char _buf[1024];
	int _len;
	int _lineNum;
	std::vector<string> _paramSeq;
};


#endif