#pragma once
#include "indexunit.h"
#include <list>
#include <map>
#include <string>
#include "IndexValue.h"
#include "TickData.h"
#include "toolsClass.h"
#include "IndexValueBase.h"
#include "StringUtil.h"
#include "BarDaily.h"
#include "AlgoTSEng.h"
using namespace std;

class AlgoTSEng;

class ATR :	public IndexUnit
{
public:
	ATR(string sec, int num, AlgoTSEng* ats);
	~ATR(void);
private:
	string name ;
	string sec;
	int daynum;
	string exchangeName;
	string date;
	AlgoTSEng* ats1;

	BarDaily* bd;
	list<double > TrueRange;
	map<string, double> ATRmap;
	int len;
	double yesATR;
public:
	virtual string getIUID();
	virtual IndexValueBase* getIndex();
	virtual IndexValueBase* getIndex(int index) ;
	virtual void computeIndex(TickData sec) ;
	void computeIndex() ;
	double getYesValue();
	virtual void addIndexDetails(IndexDetailUnit idu);        
    virtual void clear();

	int timeToHourmin(string time);
	
};

