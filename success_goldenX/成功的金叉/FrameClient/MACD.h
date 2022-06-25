#pragma once
#include "indexunit.h"
#include <list>
#include "IndexValueBase.h"
#include "AlgoTSEng.h"
#include "BarDaily.h"
using namespace std;

class AlgoTSEng;

class MACD : public IndexUnit
{
public:
	MACD(string sec, int fastLen, int slowLen, int macdLen, AlgoTSEng* ats, int min) ;
	~MACD(void);
private:
	string name ;
	string sec;
	string exchangeName ;
	string date;
	AlgoTSEng* ats1;
	int fastLength;
	int slowLength;
	int macdLength;

	string filename;
	Bar* bd;
	list<double > MACDlist;
	map<string, double> MACDmap;
	int len;
	double yesMACD;
	int min;
public:
	virtual string getIUID();
	virtual IndexValueBase* getIndex();
	virtual IndexValueBase* getIndex(int index) ;
	virtual void computeIndex(TickData sec) ;
	virtual void addIndexDetails(IndexDetailUnit idu);        
    virtual void clear();

	void computeIndex() ;
	double getYesValue();
private:
	int timeToHourmin(string time);

	double xF;
	double xS;
	double MV; 
	double xM;
};

