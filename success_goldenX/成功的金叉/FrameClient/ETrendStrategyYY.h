#pragma once
#include <string>
#include <list>
#include "StrategyUnit.h"

#include "SendToLocal.h"
#include "CloseIndexUnit.h"
#include "TickData.h"
#include "Order.h"

using namespace std;

class ETrendStrategyYY : StrategyUnit{
public:
	ETrendStrategyYY(int min, string sec1, string sec2, int lots);
	~ETrendStrategyYY(){
	}

	void setCurrentMonth(string value){
		Sec1 = value;
	}

	void setNextMonth(string value){
		Sec2 = value;
	}

	void init();
	void run();

	string getStrategyName(){
		return strategyID;
	}



private:
	

	string strategyID;
	string Sec1;
	string Sec2;
	int Min;
	string date;

	int initValue;

	
	TickData ss1;
	TickData ss2;

	//open positions's bar; record the number of the indexunit
    int currentbar;
    int lastbar;
    int indexb;

    bool bInTrade;//进入可交易区间
    bool bInTradeLastDay;//进入可交易区间的最后一天
    bool bInNewMonth;//已换约
    bool bGap;//gap is ok
    list<double> gapL;//价差List
    double high15;//if bInTrade=true, enable
    double low15;//if bInTrade=true, enable
    
       
    int lot;
    //timer timer;
    
    
    //for cancel
    int checkFre;
    double checkbegintime1;
    double checkbegintime2;
    bool bCheckCancel1;
    bool bCheckCancel2;
    Order* checkOr1;
    Order* checkOr2;
    Order* checkOr3;
    Order* checkOr4;
    bool bCon11;
    bool bCon21;
    bool bCon31;
    
    bool bCon12;
    bool bCon22;
    bool bCon32;


	void InitDD(TickData ss);
	int getCurrentBar();

	//initTimerServide();
	
	void initForStart();

	void doCancel(TickData ss);
	void doCancel_2(TickData ss);

	//SendToLocal  stl;
};