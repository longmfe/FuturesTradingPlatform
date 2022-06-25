#pragma once
#include "indexunit.h"
#include <string>
#include <list>
#include "TickData.h"
using namespace std;
class DefaultIndexUnit :
	public IndexUnit
{
public:
	DefaultIndexUnit(void);
	~DefaultIndexUnit(void);

private:
	string name ;
    list< double > indexListOpen;
    list< double > indexListClose;
    list< double > indexListHigh;
    list< double > indexListLow;
    int len ;
    int min;
    int iInitHHMM;

    int lastMin  ;
    double open ;
    double close ;
    double high ;
    double low ;
    
public:
	double inValidV;

    


    //每个BAR，以min分钟为周期
     DefaultIndexUnit(int min, int initHHMM);

   
    //默认返回open的指标，以下类似
    double getIndex();

   
    //支持正负数索引，-1倒数第一个，-2倒数第二个，0正数第一个，1正数第二个，以此类推，以下同此
    double getIndex(int index) ;
    
    double getIndexOpen() ;


    double getIndexOpen(int index)  ;
    
    double getIndexClose() ;

    double getIndexClose(int index)  ;
    
    double getIndexHigh() ;

    double getIndexHigh(int index) ;
    
    double getIndexLow() ;

    double getIndexLow(int index)  ;
 
    void computeIndex(TickData sec)  ;

    int timeToHourmin(string time) ;

  
    string getIUID() ;

   
    void clear()  ;

               
    void addIndexDetails(IndexDetailUnit idu)  ;
	

};

