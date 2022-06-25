#pragma once
#include "Order.h"
#include <string>
#include <iostream>
#include <list>
#include <map>
#include "AlgoTSEng.h"
#include "AccountManage.h"
#include "CTPClass.h"
#include "ExecReport.h"
#include "PositionManage.h"
#include "Lock.h"
#include "mLog.h"
#include "RequestStore.h"
#include "readInstanceCfg.h"
using namespace std;

class CTPClass;
class AlgoTSEng;
class AccountManage;
class PositionManage;
class SocketDama;

class AlgoManager
{
public:
	AlgoManager(bool bRealMarket , bool bRealTrade);
	~AlgoManager();

	static int  getLastOrderID();
	AccountManage* getAccountMana();
	//CTP, for Real Market, Wangzhe
	void addContracts(string, int);	
	void OrderUpdate(int , string, int);//orderid, stringid, orderstatus
	void addPosition(ExecReport er);
	void OnRspPosition(string, int , int, double ,int);//con, direction, lots, price, islast	
	void OnMarketData(string con, double lastp, double lastV, double askp, double askV,
		         double bidP, double bidV, double openI, double traddV, string date, string time);
	void OnRspAccount(double, double);//权益，可用资金
	bool sendorder(Order* order, AlgoTSEng* ae);
    bool cancelOrder(Order* order, AlgoTSEng* ae);
	void reqPositions();
	void reqAccount();
	void reqAllCons();

	void finishTheWork(bool flag, string datetime);
	void judgeDominantContract();

	CTPClass* ctp;
	mLog* pLog;
	//合约，策略指针
	map<string, list<AlgoTSEng*>> pzAlgosMap;

	//MD Real or Trade Real?
	void setMDReal(bool real);
	bool IsMDReal();
	void setRealTrade(bool real);
	bool IsTradeReal();

	map<string, map<string, double>> convol;
	list<string> args;
	toolsClass tc;
	readInstanceCfg rc;
	bool bRequestAllContrct;
private:
	
	//客户权益管理
    AccountManage* accM;
	static int initOrderID;
	bool bRealMarket;
	bool bRealTrade;

	//品种，合约列表
	map<string, list<string>> pzConsMap;
    
	//策略ID，策略指针
	map<string, AlgoTSEng*> idAlgoMap;
	//pz,(contract，tradevol)
	//map<string, double> convol;
	
		
	//for Real Market, 供内部校验使用
	PositionManage* pm;
	PositionManage* pmR;//查询得到的
	//资金校验系数 p ,初始化时手工指定
	double p;

	//list of contracts
	list<string> g_cons;

	SocketDama* psockDm;

	void init();	
	string getPz(string );	
	
};