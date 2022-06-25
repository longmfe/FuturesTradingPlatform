#pragma once
#include "indexunit.h"
#include <list>
#include <string>
#include "IndexValue.h"
#include "TickData.h"
#include "toolsClass.h"
#include "IndexValueBase.h"
#include <iostream>
using namespace std;
class CloseIndexUnit :
	public IndexUnit
{
public:
	CloseIndexUnit(void);
	~CloseIndexUnit(void);
private:
	string name ;
    list<string> paramlist; // the original list from request.
    int min; // cycle(minute)
    int iInitHHMM; // BeginTime(eg.0915) 
    string exchangeName  ;  
    //Bar info
    list<int> barSeq  ; //bar sequence
    int lastMin ; //time of current bar 
    int lastBarNum ; //Num of current bar
    //Result info
    list<IndexValue> indexListClose;//result seq
    int len  ; // length of indexListClose
    double close ;//current index value

	/*------------------------------Variable only for client--------------------------------------------------*/
    string date  ;
    string param;
    string Sec;
/*-------------------------------Reserved variable-------------------------------------------------------*/
public:
	double inValidV;	//static
private:
	int mode ; //0-using in server side ; 1- using in client side

    /**
     *Constructor for client
     */
	
	IndexValue getindexListClosebyIndex(int index1);
public:
	CloseIndexUnit(int min, int initHHMM, string sec) ;
	
	int timeToHourmin(string time) ;
	
    list<int> GetBarSeq(int InitTime,int cycle,string exName)  ;
    
    int TimeInc(int itime,int cycle) ;
   
	virtual void computeIndex(TickData sec) ;

	//only for client side
 
	virtual string getIUID();
   
	//only for client side
	 
	virtual IndexValueBase* getIndex()  ;
    
	//only for client side
	virtual IndexValueBase* getIndex(int index) ;
	
	//only for client side
    int getSize() ;
    
	//only for client side
 
    virtual void addIndexDetails(IndexDetailUnit idu)  ;
    
	//only for client side
 
	virtual void clear();
	int getbarSeq(int ibar);

};

