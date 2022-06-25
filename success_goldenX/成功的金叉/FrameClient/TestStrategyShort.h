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

class TestStrategyShort:StrategyUnit{
public:
	TestStrategyShort(AlgoTSEng* ats);
	~TestStrategyShort(void);
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
	int siChaMin; //�����ʱ���

	int maxDiffIndex;
	int tmpHighDiffIndex;

	double MinMove;
	double splitDot;  //���׻���

	bool isTradingReal;

	bool siCha();

	bool noClose;
	int bigBarNum;
	int smallBarNum;
	bool smallSiCha;

};