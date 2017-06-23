#ifndef __MSG_LINE_H
#define __MSG_LINE_H

#include "include.h"
enum LineType
{
	LineTypeInclude = 0, //����
	LineTypeBreak = 1,	//����
	LineTypeExplain = 2, //ע��	
	LineTypeClass = 3,	//��
	LineTypeFunc = 4,	//����
	LineTypeAccess = 5,	//�������η�
	LineTypeLeftBrace = 6, //������
	LineTypeRightBrace = 7, //�һ�����
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