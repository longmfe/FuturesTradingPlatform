#pragma once
#include "StrategyUnit.h"
#include "AlgoTSEng.h"
#include "toolsClass.h"
#include "CombBar.h"
#include "SendToLocal.h"

class AlgoTSEng;
class SendToLocal;

extern enum Stype;

class FullgoalStrategy:StrategyUnit{
public:
	FullgoalStrategy(AlgoTSEng* ats);
	~FullgoalStrategy(void);

	void run();

private:
	AlgoTSEng* ats;	
	string strategyID;
	TickData ss;
	CombBar* cb;
	SendToLocal* stl;
	bool isTradingReal;

	int maNum;
	int N_1minBar;
	double MAparam_price;
	double MAparam_vol;
	double MAparam_price_c;   //价差平仓阀值
	double MAparam_vol_c;     //成交量之和平仓阀值
	double stopLoss;
	string contract1;
	string contract2;
	toolsClass tc;

	bool hasOpen;
	bool hasClose;
	bool hasOpen2;
	string openTime;
	string closeTime;
	double openDiff;
	int clearTime; 

	void init();	
	void esSendOrder(Order* order);
	void esSendCombOrder(Stype direction1, double price1, int volume1, Stype direction2, double price2, int volume2); //1-limit 0-market
	void esOpenCombOrder(int direction, bool type, int volume1, int volume2);
	void esCloseCombOrder(int direction, bool type, int volume1, int volume2);
	bool esCancleAllOrder(string contract);
};