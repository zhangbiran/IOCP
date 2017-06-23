#include "func.h"
#include "line.h"
#include "macrodef.h"
#include "KeywordManager.h"

Func::Func()
{

}

bool Func::analyze()
{
	if (_line->_paramSeq.size() < 3)
	{
		_line->printError();
		THROW_EXCEPTION("");
		return false;
	}

	if (_line->_paramSeq.back() != ";")
	{
		_line->printError();
		THROW_EXCEPTION("");
		return false;
	}

	_funcName = _line->_paramSeq[1];
	_funcPrototype = _funcName;
	for (int i = 2; i < _line->_paramSeq.size() - 1; i += 2)
	{
		string keyWord = _line->_paramSeq[i];
		if (!CKeyWordManager::instance()->isKeyWord(keyWord))
		{
			_line->printError();
			THROW_EXCEPTION("no the keyword");
		}
		_funcPrototype += _line->_paramSeq[i];
	}

	cout << "_funcPrototype: " << _funcPrototype << endl;
	return true;
}

bool Func::writeH(string& str)
{
	str += "\tvoid ";
	str += _funcName;
	str += "(";
	for (int i = 2; i < _line->_paramSeq.size() - 1; i+=2)
	{
		if (i > 2)
		{
			str += ",";
		}
		str += _line->_paramSeq[i];
		_funcPrototype += _line->_paramSeq[i];
		str += " ";
		str += _line->_paramSeq[i + 1];
		str += "";
	}
	str += ");\n";

	return true;
}
bool Func::writeCPP(string& str)
{
	str += _funcName;
	str += "(";

	for (int i = 2; i < _line->_paramSeq.size() - 1; i += 2)
	{
		if (i > 2)
		{
			str += ",";
		}
		str += _line->_paramSeq[i];
		str += " ";
		str += _line->_paramSeq[i + 1];
		str += "";
	}
	str += ")\n";
	str += "{\n";
	str += "\n";
	str += "}";

	return true;
}
