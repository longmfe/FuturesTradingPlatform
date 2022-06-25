#pragma once
#include "indexunit.h"
#include <list>
#include "IndexValueBase.h"
using namespace std;
class OpenIndexUnit :
	public IndexUnit
{
public:
	OpenIndexUnit(void);
	~OpenIndexUnit(void);
	/*------------------------------Generic Variable---------------------------------------------------------*/
    //Index property
private:
	string name ;
	list<string> paramlist; // the original list from request.
	int min; // cycle(minute)
	int iInitHHMM; // BeginTime(eg.0915) 
	string exchangeName ; 
	//Bar info
	list<int> barSeq ; //bar sequence
	int lastMin ; //time of current bar 
	int lastBarNum; //Num of current bar
	//Result info
	list<int> indexListOpen;//result seq
	int len; // length of indexListClose
    double open;//current index value
/*------------------------------Variable only for client--------------------------------------------------*/
    string date;
    string param;
    string Sec;
/*-------------------------------Reserved variable-------------------------------------------------------*/
public:
	double inValidV;	
	
	/**
	 *Constructor for server
	 */
	OpenIndexUnit(int min, int initHHMM, string sec) ;

	int timeToHourmin(string time) ;
	list<int>  GetBarSeq(int InitTime,int cycle,string exName) ;
	int TimeInc(int itime,int cycle) ;
	void computeIndex(TickData sec) ;

	//only for client side
 
	string getIUID() ;
	
    //only for client side
 
    // 
	virtual IndexValueBase* getIndex() ;
	virtual IndexValueBase* getIndex(int index) ;
	//only for client side
	int getSize() ;
	//only for client side
	void addIndexDetails(IndexDetailUnit idu) ;
	void clear() ;
	
    
};

