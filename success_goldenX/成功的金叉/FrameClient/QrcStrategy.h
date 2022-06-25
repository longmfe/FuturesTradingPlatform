#pragma once
#include <string>
#include <list>
#include "StrategyUnit.h"
#include "SendToLocal.h"
#include "Bar.h"
#include "BarValue.h"
#include "TickData.h"
#include "Order.h"
#include "QRCadapter.h"
using namespace std;
class AlgoTSEng;
class QRCadapter;
class SendToLocal;

class QrcStrategy : StrategyUnit{
public:
	QrcStrategy(AlgoTSEng* ats);
	~QrcStrategy(void);
private:
	AlgoTSEng* ats;
	QRCadapter* qrc;
	string strategyID;
	toolsClass tc;
public:
	 void init();
	 void run();
	 int getQMState(string qrcmsg); 
	 int ProcessQrcRtn(string qrcmsg);
	 Order* String2Order(string strSendOrder);


	 //added 2013-3-13 wz
	 int thisQMstatus;
	 list<Order* > thisQM;
	 int lastQMstatus;
	 list<Order* >  lastQM;
	 int getQMStatus(string qmsg);
	 int placeQrcOrders_firstTime(list<Order* > ol);
	 int placeQrcOrders_again(list<Order* > ol);
	 int judgeOrder(Order *);

	 void mySendOrder(Order* order);
	 bool myCancelAll();
	 bool myCancelAll(string con);
	 bool myCancelSingle(int orID);

	 int myPositionReport(string sec, int position);

	 string contractNameTrans(string qrccon);

	 void UnitTest(int i);

	 int initReqtoQRC(double x);

	 int preFeedin();

	 int preLoad();

	 string dummyFeedin(BarValue bv,double position,bool isEnd);

	 string minutePP(string time);
private:
	TickData ss;
	string Nowdate;
	int Nowtime;
	Bar* b1;
	BarValue* bv1;
	int barcount ;

	int conCount;
	int NowconCount;
	SendToLocal* stl;

	/***********  Caution , set the parameters carefully   **********/
	bool isTradingReal;
	bool isTickFeedin;
	bool isBarFeedin;
	bool isTickTrade;
	bool isBarTrade;
	bool ispreloadTrade;
	/*****************************************************************/
};