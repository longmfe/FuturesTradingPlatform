#pragma once
#include "indexunit.h"
#include "GlobalDataManage.h"
class DuMaIndexUnit :
	public IndexUnit
{
public:
	DuMaIndexUnit(void);
	~DuMaIndexUnit(void);
	string name ;
    list< double> indexList;
    int len ;
    int min;

    int lastMin  ;
    double inValidV ;

    double open ;
    double close  ;
    double high ;
    double low ;

    int circle ;
    DuMaIndexUnit(string id, int min, int cir) ;
    double  getIndex() ;
    double getIndex(int index)  ;

    void computeIndex(TickData sec) ;

    int timeToHourmin(string time) ;
    string getIUID()  ;
    void clear()  ;
    void addIndexDetails(IndexDetailUnit idu) ;
};

