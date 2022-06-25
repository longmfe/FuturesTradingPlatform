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
	void OnRspAccount(double, double);//Ȩ�棬�����ʽ�
	bool sendorder(Order* order, AlgoTSEng* ae);
    bool cancelOrder(Order* order, AlgoTSEng* ae);
	void reqPositions();
	void reqAccount();
	void reqAllCons();

	void finishTheWork(bool flag, string datetime);
	void judgeDominantContract();

	CTPClass* ctp;
	mLog* pLog;
	//��Լ������ָ��
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
	
	//�ͻ�Ȩ�����
    AccountManage* accM;
	static int initOrderID;
	bool bRealMarket;
	bool bRealTrade;

	//Ʒ�֣���Լ�б�
	map<string, list<string>> pzConsMap;
    
	//����ID������ָ��
	map<string, AlgoTSEng*> idAlgoMap;
	//pz,(contract��tradevol)
	//map<string, double> convol;
	
		
	//for Real Market, ���ڲ�У��ʹ��
	PositionManage* pm;
	PositionManage* pmR;//��ѯ�õ���
	//�ʽ�У��ϵ�� p ,��ʼ��ʱ�ֹ�ָ��
	double p;

	//list of contracts
	list<string> g_cons;

	SocketDama* psockDm;

	void init();	
	string getPz(string );	
	
};