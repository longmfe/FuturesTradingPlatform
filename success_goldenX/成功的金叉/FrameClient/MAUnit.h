#pragma once
#include "indexunit.h"
#include "GlobalDataManage.h"
#include <list>

class MAUnit :
	public IndexUnit
{
private:
	//Bar* bar;

public:
	MAUnit(string,int,int,GlobalDataManage*);
	~MAUnit(void);
	string name ;
	list< double > indexList;
	int len ;
	int min;
	int num;
	double curMAV;

	int lastMin ;
	double inValidV  ;
	GlobalDataManage* pGlobalDm;

	double getCurMAV();

	double getMAByIndex();

	double getMAByIndex(int index)  ;

	virtual IndexValueBase* getIndex();
	virtual IndexValueBase* getIndex(int index) ;

	void computeIndex(TickData sec)  ;

	int timeToHourmin(string time);

	string getIUID() ;

	void clear() ;

	void addIndexDetails(IndexDetailUnit idu) ;
};

