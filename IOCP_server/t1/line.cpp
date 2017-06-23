#include "line.h"
Line::Line()
{
	memset(_buf, 0, 1024);
}

bool Line::read(int lineNum, fstream& f)
{
	if (!f.is_open())
	{
		return false;
	}

	f.getline(_buf, 1024);
	_lineNum = lineNum;
	if (!f.good())
	{
		return false;
	}

	return true;
}

void Line::printLine()
{
	//cout << _buf << endl;
	cout << "lineNum:" << _lineNum << _buf << strlen(_buf) << endl;
}

void Line::split(char *tokens)
{
	char *p = strtok(_buf, tokens);
	while (p != NULL)
	{
		//cout << p << endl;
		string s(p);
		//cout << s << s.length() << endl;
		_paramSeq.push_back(s);
		p = strtok(NULL, tokens);
	}
	//cout << _buf << endl;
	//cout << "--------------------------------" << endl;

}

int Line::getType()
{
	if (_paramSeq.empty())
	{
		return LineTypeBreak;
	}

	if (_paramSeq[0] == string("#include"))
	{
		return LineTypeInclude;
	}
	else if (_paramSeq[0] == string("class"))
	{
		return LineTypeClass;
	}
	else if (_paramSeq[0] == string("void"))
	{
		return LineTypeFunc;
	}
	else if (_paramSeq[0] == string("{"))
	{
		return LineTypeLeftBrace;
	}
	else if (_paramSeq[0] == string("};"))
	{
		return LineTypeRightBrace;
	}
	else if (_paramSeq[0] == string("public:"))
	{
		return LineTypeAccess;
	}
	else
	{
		if (_paramSeq[0].size() < 2)
		{
			return -1;
		}
		else
		{
			const char *p = _paramSeq[0].c_str();
			if (*p == '/' && *(p + 1) == '/')
			{
				return LineTypeExplain;
			}

			return -1;
		}
	}


	return -1;
}

void Line::printError()
{
	cout << "syntax error: " << "file:" << __FILE__ << ", line:" << _lineNum << endl;
}