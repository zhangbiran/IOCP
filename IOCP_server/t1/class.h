#ifndef __MSG_CLASS_H
#define __MSG_CLASS_H
#include "include.h"
#include "func.h"
#include "line.h"


class MsgClass
{
public:
	MsgClass() {}
	MsgClass(Line *line);
	void addFunc(Line* line);
	virtual bool analysis();
	virtual bool writeH(string& str);
	virtual bool writeCPP(string& str);

	std::string _className;
	Line * _line;
	//std::vector<Line*> _lineSeq;
	std::vector<Func*> _funcSeq;
	std::set<string> _funcPrototypeSeq;
	
};

class LineClass : public MsgClass
{
public:
	virtual bool analysis() { return true; }
	virtual bool writeH(string& str);
	virtual bool writeCPP(string& str);

	Line * _line;
};


#endif