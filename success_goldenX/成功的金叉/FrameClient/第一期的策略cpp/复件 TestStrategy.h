#pragma once
#include <string>
#include <list>
#include "StrategyUnit.h"
#include "SendToLocal.h"
#include "Bar.h"
#include "BarValue.h"
#include "TickData.h"
#include "Order.h"
#include "ATR.h"
#include "MACD.h"
using namespace std;
class AlgoTSEng;
class SendToLocal;
class ATR;
class MACD;

class TestStrategy:StrategyUnit{
public:
	TestStrategy(AlgoTSEng* ats);
	~TestStrategy(void);
private:
	AlgoTSEng* ats;
	string strategyID;
	toolsClass tc;
public:
	void init();
	void run();
	void esSendOrder(Order* order);
private:
	SendToLocal* stl;
	TickData ss;
	double preDiff;
	double curDiff;
	bool hasTrade;
	int jinChaMin; //金叉的时间点

	int maxDiffIndex;
	int tmpHighDiffIndex;

	double MinMove;
	double splitDot;  //交易滑点

	bool isTradingReal;

	bool jinCha();

	bool noClose;
	int bigBarNum;
	int smallBarNum;
	bool smallJinCha;

};