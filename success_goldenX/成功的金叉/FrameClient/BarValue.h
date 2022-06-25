#pragma once
#include <string>
#include <iostream>
#include"IndexValueBase.h"
using namespace std;

class BarValue:
	public IndexValueBase
{
public:
	BarValue(void);
	~BarValue(void);
	BarValue(string date,string time,double open,double close,
	double high,double low,double tradevolume,double openinterest,string realContractID,string nameContractID);

	void setDiff(double);
	double getDiff();

public:
	string date;
	string time;
	double open;
	double close;
	double high;
	double low;
	double tradevolume;
	double openinterest;
	//add 2 fields 
	string realContractID ;   //like IF1306
	string nameContractID ;   //like IF888

	string separator;
	string Object2String();
	//void String2Object(string str);

	double macdDiff;


};