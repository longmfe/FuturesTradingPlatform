#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "toolsClass.h"
using namespace std;
class KeepRecord
{
public:
	KeepRecord();
	~KeepRecord(void);
private:
	string filepath ;
    string dir;
    string filepostfix ;
    string splitField;
    ofstream* fw;

    string fileName;
    bool isInital;
public:
	KeepRecord(string name);
	
    void init();
	

	
    void appendData(string time,double lastPrice,double lVolume,
                            double BidPrice,double BidVolume,double AskPrice,
                            double AskVolume,double OpenInterest,double TradeVolume);
    
    void appendData(string rec);
    
    void Finish();
	
private:
	void printInit();

	
public:
	void writeLine(string parm);
};

