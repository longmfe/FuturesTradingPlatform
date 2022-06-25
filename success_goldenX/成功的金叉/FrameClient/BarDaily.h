#pragma once
#include <string>
#include <list>
#include "IndexUnit.h"
#include "toolsClass.h"
#include "TickData.h"
#include"BarValue.h"
#include "IndexValueBase.h"
#include <iostream>
#include <fstream>
using namespace std;

class  BarDaily : public IndexUnit 
{
public:
	BarDaily(string exname,string sec) ;
	~BarDaily(void);

	int timeToHourmin(string time) ;
	//list<int> GetBarSeq(int InitTime,int cycle,string exName)  ;
	virtual string getIUID();
	virtual IndexValueBase* getIndex()  ;
	virtual IndexValueBase* getIndex(int index);
	virtual void computeIndex(TickData sec) ;
	virtual void addIndexDetails(IndexDetailUnit idu);
	virtual void clear();
	
	int getlen();
	void readBDfromFile();
	void writeBDtoFile(string thesec);
	void addTodayBD(string date, double value);

private:
	string name;
	string secName;
	string exchangeName; 
	string Sec;
	string date;
	string filename;
	string strSep;

	toolsClass tc;

	//Bar info
	list<int> barSeq; //bar sequence
	int lastMin; //time of current bar 
	int lastBarNum; //Num of current bar
	bool bwriteBD;

	//Result info
	list<BarValue> indexListBar;//result seq
	int len; // length of indexListClose
	double open;//current index value
	double close ;//current index value
	double high;//current index value
	double low;//current index value
	double tradevolume ;//current index value
	double openinterest;//current index value

private:
	BarValue getindexListBarbyIndex(int index1);
	string getPz(string con);
};

