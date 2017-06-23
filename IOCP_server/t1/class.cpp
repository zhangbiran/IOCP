#include "class.h"
#include "ClassManager.h"
#include "macrodef.h"

MsgClass::MsgClass(Line *line) : _line(line)
{
}

void MsgClass::addFunc(Line* line)
{
	//_paramSeq.push_back(line);
	Func * func = new Func();
	func->_line = line;
	_funcSeq.push_back(func);
}


bool MsgClass::analysis()
{
	if (_line->_paramSeq.size() != 2)
	{
		_line->printError();
		return false;
	}

	_className = _line->_paramSeq[1];
	if (ClassManager::instance()->isExistClassName(_className))
	{
		_line->printError();
		THROW_EXCEPTION("exist class name");
		return false;
	}

	ClassManager::instance()->addClassName(_className);

	for (std::vector<Func*>::iterator it = _funcSeq.begin();
		it != _funcSeq.end(); ++it)
	{
		(*it)->analyze();
		if (_funcPrototypeSeq.count((*it)->_funcPrototype) > 0)
		{
			(*it)->_line->printError();
			THROW_EXCEPTION("exist function");
		}
		_funcPrototypeSeq.insert((*it)->_funcPrototype);
	}

	return true;
}

bool MsgClass::writeH(string& str)
{
	str += "class ";
	str += _className;
	str += '\n';
	str += "{\n";
	str += "public:\n";
	for (std::vector<Func*>::iterator it = _funcSeq.begin();
		it != _funcSeq.end(); ++it)
	{
		(*it)->writeH(str);
	}

	str += "};\n";

	return true;
}

bool MsgClass::writeCPP(string& str)
{
	for (std::vector<Func*>::iterator it = _funcSeq.begin();
		it != _funcSeq.end(); ++it)
	{
		str += "void ";
		str += _className;
		str += "::";
		(*it)->writeCPP(str);
	}
	return true;
}

bool LineClass::writeH(string& str)
{
	str += _line->_buf;
	str += "\n";
	return true;
}

bool LineClass::writeCPP(string& str)
{
	return true;
}
