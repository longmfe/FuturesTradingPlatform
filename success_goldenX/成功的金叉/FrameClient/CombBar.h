#pragma once
#include "IndexUnit.h"
#include "StringUtil.h"
#include "Bar.h"


class CombBar : public IndexUnit
{
public:
	CombBar(int inithhmm, Bar* b1, Bar* b2, int maNum);
	~CombBar(void);

	void init();

private:
	//StringUtil su;
	toolsClass tc;

	int maNum;
	Bar* bar1;
	Bar* bar2;

	int lastMin;
	int lastBarNum;

	string contract1;
	string contract2;
	int inithhmm;

	double price1;
	double price2;
	double priceDiffer;
	

	double volume1;
	double volume2;
	double volumeSum;

public:
	vector<double> differVector;
	vector<double> maDifferV;

	vector<double> sumVector;
	vector<double> maSumV;

	double getDifferByIndex(int index);
	double getSumByIndex(int index);
	int getDifferVsize();

	virtual void computeIndex(TickData sec) ;

	//only for client side
	virtual string getIUID();
	//only for client side
	virtual IndexValueBase* getIndex();
	virtual IndexValueBase* getIndex(int index);
	//only for client side
	virtual void addIndexDetails(IndexDetailUnit idu);
	virtual void clear();

	double getBidPrice1();
	double getAskPrice1();
	double getBidPrice2();
	double getAskPrice2();
};

