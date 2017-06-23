#include "file.h"
#include "macrodef.h"
MsgFile::MsgFile()
{

}

MsgFile::~MsgFile()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

bool MsgFile::open(char *fileName)
{
	if (_file.is_open())
	{
		return false;
	}

	_file.open(fileName);
	if (!_file.is_open())
	{
		return false;
	}

	_fileName = fileName;

	return true;
}

bool MsgFile::open(char *fileName, int openMode)
{
	if (_file.is_open())
	{
		return false;
	}

	_file.open(fileName, openMode);
	if (!_file.is_open())
	{
		return false;
	}

	_fileName = fileName;

	return true;
}

void MsgFile::close()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

void MsgFile::clear()
{
	_lineSeq.clear();
	_classSeq.clear();
	_fileName = "";
}
void MsgFile::readAllLine()
{
	int index = 0;
	do 
	{
		Line *line = new Line();
		bool ret = line->read(++index, _file);
		if (ret)
		{
			_lineSeq.push_back(line);
		}
		else
		{
			if (_file.eof())
			{
				_lineSeq.push_back(line);
			}
			break;
		}
	} while (1);
}

void MsgFile::printAllLine()
{
	for (std::vector<Line*>::iterator it = _lineSeq.begin();
		it != _lineSeq.end(); ++it)
	{
		(*it)->printLine();
		(*it)->split("\t ,()");
	}
}

bool MsgFile::organization()
{
	for (std::vector<Line* >::iterator it = _lineSeq.begin();
		it != _lineSeq.end();)
	{
		int type = (*it)->getType();
		if (type == LineTypeInclude ||
			type == LineTypeBreak ||
			type == LineTypeExplain)
		{
			LineClass * newClass = new LineClass();
			newClass->_line = *it;
			_classSeq.push_back(newClass);
			++it;
		}
		else if (type == LineTypeClass)
		{
			MsgClass * newClass = new MsgClass(*it);
			it += 1;
			if (it == _lineSeq.end())
			{
				(*(it - 1))->printError();
				return false;
			}
			if ((*it)->getType() != LineTypeLeftBrace)
			{
				(*it)->printError();
				return false;
			}

			it += 1;
			while (1)
			{
				if (it == _lineSeq.end())
				{
					(*(it - 1))->printError();
					return false;
				}
				if ((*it)->getType() == LineTypeRightBrace)
				{
					_classSeq.push_back(newClass);
					++it;
					break;
				}
				else if ((*it)->getType() == LineTypeFunc)
				{
					newClass->addFunc(*it);
					++it;
				}
				else
				{
					(*it)->printError();
					return false;
				}
			}
		}
		else
		{
			(*it)->printError();
			return false;
		}
	}

	return true;
}

bool MsgFile::analyze()
{
	for (std::vector<MsgClass* >::iterator it = _classSeq.begin();
		it != _classSeq.end(); ++it)
	{
		(*it)->analysis();
		CCustomKeyWord * keyWord = new CCustomKeyWord((*it)->_className);
		CKeyWordManager::instance()->addKeyWord(keyWord);
	}

	return true;
}

void MsgFile::writeH()
{
	size_t pos = _fileName.find(".");
	if (-1 == pos)
	{
		cout << "no find" << endl;
		return;
	}

	string fileName(_fileName, 0, pos);
	fileName += ".h";
	cout << fileName << endl;
	
	string str;
	for (std::vector<MsgClass* >::iterator it = _classSeq.begin();
		it != _classSeq.end(); ++it)
	{
		(*it)->writeH(str);
	}

	fstream f;
	f.open(fileName.c_str(), ios_base::in | ios_base::out | ios_base::trunc);
	f.write(str.c_str(), str.length());
	f.close();
}
void MsgFile::writeCPP()
{
	size_t pos = _fileName.find(".");
	if (-1 == pos)
	{
		cout << "no find" << endl;
		return;
	}

	string fileName(_fileName, 0, pos);
	

	string str;
	
	str += "#include \"";
	str += fileName;
	str += ".h\"\n";
		
		fileName += ".cpp";
	cout << fileName << endl;

	for (std::vector<MsgClass* >::iterator it = _classSeq.begin();
		it != _classSeq.end(); ++it)
	{
		(*it)->writeCPP(str);
	}

	fstream f;
	f.open(fileName.c_str(), ios_base::in | ios_base::out | ios_base::trunc);
	f.write(str.c_str(), str.length());
	f.close();
}
