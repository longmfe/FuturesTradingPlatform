#pragma once
#include "SocketDama.h"
#include "FlowManage.h"
#include "ComputeIndexUnit.h"
#include "DealOrdersLocal.h"
#include "FlowUnits.h"
#include "DealSendedOrderUnit.h"
#include "ComputeIndex.h"
#include "ExecReport.h"
//#include "CTPClass.h"
#include "AlgoManager.h"
#include "TickData.h"
#include "ESproductInfo.h"
#include "GlobalDataManage.h"
#include "TestStrategy.h"
#include "TestStrategyShort.h"
#include "afxmt.h"  
#include <string>
#include "Lock.h"
using namespace std;
class TestStrategy;
class TestStrategyShort;
class QrcStrategy;
class GlobalDataManage;
class ESproductInfo;
class AlgoManager;
class SocketDama;

class AlgoTSEng{
public:
	GlobalDataManage* pGlobalDm;
	ESproductInfo* espi ;
	TestStrategy* esall;
	TestStrategyShort* esallShort;

	AlgoTSEng(string qpistr,AlgoManager* am,bool tradeFlag,bool mdFlag);
	~AlgoTSEng();
	void initService();
	void process(string& con, double& lastp, double& lastV, double& askp, double& askV,
		double& bidP, double& bidV, double& openI, double& traddV, string& date
		, string& time);
	void process(string con, double lastp, double lastV, double askp, double askV,
		double bidP, double bidV, double openI, double traddV, string date
		,  string time, int xx);

	//get TickData for this Algo
	void processMarketEvent(TickData*);

	void finishTodayWork();
	void CancelMarketData();
	void addPosition(ExecReport er);
	
	//test from qrc, not use now
	map<string,TickData*> cmap ;
	int initCmap_test(string pn,string date);
	int putContract_cmap_test(string pn);
	void SimulateDayChanged(string yesterday,string today);
	
	//not use for ES_A
	int addContract(list<string> clist);

	toolsClass tc;
	CLock mlock;
	list<TickData*> tickl;

	//usefull??
	int sendSingleDummy(int curHHMMSS);
private:
	FlowManage* fm;
	bool IsrealMD ;
	bool IsrealTrade;
	
public:
	bool IsContractsAdded;
	bool IsPreBarFeeded;
	bool IsMorningDummyFeeded;
	bool IsEveDummyFeeded;
	static const int DummyNum = 5;
	int DummyCount;

};