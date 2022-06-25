#pragma once
#include <string>
using namespace std;
class IndexDetailUnit
{
public:
 
	~IndexDetailUnit(void);
	//指标值的四维信息
private:
	string Date;
    string Time;
    int Seq;
    double Value;
public:
	IndexDetailUnit(string date, string time, int seq, double value);
    
    string getDate();
    
    string getTime();
    
    int getSeq();
    
    double getValue();

};

