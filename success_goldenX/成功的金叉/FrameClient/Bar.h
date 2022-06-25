#pragma once
#include <string>
#include <list>
#include "IndexUnit.h"
#include "toolsClass.h"
#include "TickData.h"
#include <iostream>
#include <fstream>
#include"BarValue.h"
#include "IndexValueBase.h"
using namespace std;

class  Bar : public IndexUnit 
{
public:
	//Bar(void);
	Bar(list<string> paramlist) ;
	Bar(int min, int initHHMM, std::string sec) ;
	~Bar(void);

private:
	string name;
	list<string> paramlist; // the original list from request.
	int min; // cycle(minute)
	int iInitHHMM; // BeginTime(eg.0915) 
	string exchangeName; 
	//Bar info
	list<int> barSeq; //bar sequence
	int lastMin; //time of current bar 
	int lastBarNum; //Num of current bar
	//Result info
	list<BarValue*> indexListBar;//result seq
	
	int len; // length of indexListClose
	double open;//current index value
	double close ;//current index value
	double high;//current index value
	double low;//current index value
	double tradevolume ;//current index value
	double openinterest;//current index value
	/*------------------------------Variable only for client--------------------------------------------------*/
private:
	string date;
	string param;
	string Sec;
	bool IsNumInc;
	bool isFirst;  //is first tick of current barValue
public:
	double inValidV;	// static 
	list<bool> hasOpenList;
	list<bool> hasOpenShortList;
public:
	int getLastBarNum();
	int timeToHourmin(string time) ;
	list<int> GetBarSeq(int InitTime,int cycle,string exName)  ;
	int TimeInc(int itime,int cycle) ;
	virtual void computeIndex(TickData sec) ;

	//only for client side
	virtual string getIUID();
	//only for client side
	virtual IndexValueBase* getIndex()  ;
	virtual IndexValueBase* getIndex(int index);
	int getSize();
	//only for client side
	virtual void addIndexDetails(IndexDetailUnit idu);
	virtual void clear();

	//added by wz 2013-3-8
	bool getIncStatus();
	void setIncKnown();
	//IndexValueBase* getBVbyHHMM(string HHMM);
	string getSec();
	//
	bool Iszerofill;

	void writeBartoFile(string sec);
	bool getHasOpen(int index);
	bool getLastFlag();
	bool getHasOpenShort(int index);
	bool getLastShortFlag();
	bool getIsFirst();

private:
	BarValue* getindexListBarbyIndex(int index1);
	void readBarfromFile();
	
	string strSep;
	toolsClass tc;
	string filename;
	string getPz(string con);
};


