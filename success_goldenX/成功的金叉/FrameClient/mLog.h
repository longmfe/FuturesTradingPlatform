#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "UtilFun.h"
using namespace std;

class mLog
{
public:
	mLog(string filename, bool bwrite)
	{
		fname = "./" + filename;
		bWrite = bwrite;
		if(!bWrite)
		{
			return;
		}
		of.open(fname, ios::app);
		UtilFun uf;
		of << uf.getCHCurDate() << "-----Begin" << endl;
		
	}
	~mLog()
	{
		if(!bWrite)
		{
			return;
		}
		UtilFun uf;
		of << uf.getCHCurDate() << "-----End" << endl;
		of.close();
	}

	void addlog(string str)
	{
		if(!bWrite)
		{
			return;
		}
		UtilFun uf;
		of << uf.getCHCurDate() << "-----" << str << endl;
	}
private:
	string fname;
	ofstream of;
	bool bWrite;
};