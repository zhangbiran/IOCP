#ifndef __MSG_FILE_H
#define __MSG_FILE_H
#include "include.h"

#include "class.h"


class MsgFile
{
public:
	bool open(char *fileName);
	bool open(char *fileName, int openMode);
	void close();
	void clear();
	void readAllLine();
	void printAllLine();
	bool organization();
	bool analyze();
	void writeH();
	void writeCPP();
	MsgFile();
	~MsgFile();
private:

	string _fileName;
	std::vector<MsgClass* > _classSeq;
	std::vector<Line* > _lineSeq;

	fstream _file;
};

#endif