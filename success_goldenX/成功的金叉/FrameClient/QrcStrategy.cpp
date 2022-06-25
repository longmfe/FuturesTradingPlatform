/*
*2013-4-24 Aaron
* 重构Run函数
*/
#include "stdafx.h"
#include "QrcStrategy.h"
#include <string>
#include <list>
#include "TickData.h"
#include "Order.h"
#include "QRCadapter.h"
#include "BarValue.h"
#include "toolsClass.h"
#include "SendToLocal.h"
#include <iostream>
#include<string.h>
#include<io.h>
#include   <direct.h> 
using namespace std;

extern AlgoManager* algomanager;
const string orderSignal = "[SendOrder]";

QrcStrategy::QrcStrategy(AlgoTSEng* ats)
{
	this->ats = ats;

	init();
}
QrcStrategy::~QrcStrategy(void)
{
	this->qrc->~QRCadapter();
}
void QrcStrategy::init()
{
	this->Nowdate="";
	this->Nowtime=0;
	this->barcount = 0;
	this->conCount = 1;

	stl = new SendToLocal(ats->pGlobalDm);

	/***********  Caution , set the parameters carefully   **********/
	isTradingReal = ats->IsTradeReal();
	/*
	isTickFeedin = true;
	isBarFeedin = true;
	isTickTrade = true;
	isBarTrade = false;
	*/
	isTickFeedin = false;
	isBarFeedin = true;
	isTickTrade = false;
	isBarTrade = true;
	ispreloadTrade = true;
	/*****************************************************************/

	strategyID = ats->qpi->strategyID;
	//1. Status Machine for Order sending.
	thisQMstatus = -1;
	lastQMstatus = -1;
	//2. QRCadapter
	//qrc = new QRCadapter(*(ats->qpi));
	qrc = new QRCadapter(ats->qpi);
	qrc->connect();
	qrc->initReq(algomanager->X);
}
void QrcStrategy::run()
{
	if((!ats->IsContractsAdded) || (!ats->IsPreBarFeeded)||(!ats->IsMorningDummyFeeded))
	{
		return;
	}
	ss = *(ats->pGlobalDm->getMdMana()->getTickData());
	//step 1.  Update Ticker for cmap
	/*
	//in Order to make it faster, Comment it out temporarily
	if(ats->cmap.find(ss.sec)!=ats->cmap.end())             
	{
	ats->cmap.find(ss.sec)->second->fill(ss);
	}
	else
	{
	//cout<<"Error to fill in cmap,sec is:"<< ss.sec << endl;
	return;
	}
	*/
	//Step 2.  Day Changed Handler
	if(Nowdate.compare("")==0)
	{
		Nowdate = ss.getDate();
		Nowtime = this->tc.timeToHourmin(ss.getTime());
		conCount = ats->pGlobalDm->getIndexMana()->indexMap.size();
		if(!ats->IsMDReal())
		{
			ats->SimulateDayChanged("",Nowdate);
		}
	}
	else if(ss.getDate().compare(Nowdate)!=0)
	{
		string ye = tc.doubleToString(ats->pGlobalDm->getAccountMana()->getEquity());
		ats->pGlobalDm->keepRecord(FileType::Equity,Nowdate+","+ye);
		if(!ats->IsMDReal())
		{
			ats->SimulateDayChanged(Nowdate,ss.getDate());
		}
		Nowdate = ss.getDate();
		Nowtime = this->tc.timeToHourmin(ss.getTime());
		conCount = ats->pGlobalDm->getIndexMana()->indexMap.size();
	}

	//step 3 Computer Position 
	Position* pos = ats->pGlobalDm->getPositionMana()->getPositionBysec(ss.sec);
	int posvol = pos->getBuyopen() - pos->getSellopen();

	//step 4  Bar Changed Handler
	if(Nowtime == this->tc.timeToHourmin(ss.getTime()))  //Just tick feed in needed
	{
		//MD feed in
		if(isTickFeedin)
		{
			string tickerResult = qrc->MDFeedin(ss,posvol);
			if(this->isTickTrade)
			{
				ProcessQrcRtn(tickerResult);
			}
		}
	}
	else  if(Nowtime < this->tc.timeToHourmin(ss.getTime()))//BarFeed in first,then feed current tick
	{
		//Do I need to do a more stringent check in the future? like 091100,then 091200,neither 091300 or 091000?

		//Bar feed in
		if(this->isBarFeedin){
			NowconCount = 0;
			for(map<string, IndexUnit* > ::iterator it = ats->pGlobalDm->getIndexMana()->indexMap.begin(); it != ats->pGlobalDm->getIndexMana()->indexMap.end(); it++ )
			{
				NowconCount++;
				b1 = (Bar*)(it->second);
				if(b1 == NULL )
				{ 
					continue;
				}
				if(b1->getSize()<=0)
				{
					continue;
				}
				else
				{
					if(b1->getSec().compare(ss.getSec())==0)
					{
						bv1 = (BarValue*)(b1->getIndex(-2)); //be carefully 
					}
					else
					{
						bv1 = (BarValue*)(b1->getIndex(-1));
					}

					if(this->tc.timeToHourmin(bv1->time) != Nowtime)
					{
						bv1->time = tc.IntToString(Nowtime);
						bv1->open = bv1->close;
						bv1->high = bv1->close;
						bv1->low = bv1->close;
						bv1->tradevolume = 0;
						cout<< "Bar complement!!!"<<bv1->realContractID<<"_"<<bv1->date <<"_"<< bv1->time <<endl;
					}
					if(bv1->time.length()<5)
					{
						cout<< "bv1 time error:"<< bv1->time <<endl;
					}
					string bv1timetmp = minutePP(bv1->time);
					bv1->time = bv1timetmp;
					if(NowconCount < conCount)
					{

						qrc->BarFeedin(*bv1,posvol,false);

					}
					else
					{
						string barResult = qrc->BarFeedin(*bv1,posvol,true);
						if(this->isBarTrade)
						{
							ProcessQrcRtn(barResult);
						}
					}
				}
			}
		}
		// Current ticker feedin
		if(isTickFeedin)
		{
			string tickerResult = qrc->MDFeedin(ss,posvol);
			if(this->isTickTrade)
			{
				ProcessQrcRtn(tickerResult);
			}
			//TODO rtn processSignal
		}
		Nowtime = this->tc.timeToHourmin(ss.getTime());
	}
	else  //tick coming in inverted sequence,error!
	{
		cout<<"error time inverted sequence!!!"<< "preHHMM:"<<Nowtime<<" curHHMM:"<<this->tc.timeToHourmin(ss.getTime())<<endl;
		return;
	}	

}

int QrcStrategy::ProcessQrcRtn(string qrcmsg)
{
	/*step 1 parse the QRC rtn msg*/
	//example 1:[StrategyStatus],TickBasedSignal|[StrategyStatus],CancelLimitOrders  Aaron:it's a heart beat message,means we don't need do anything.
	//example 2: 2013-03-06 13:47:15.229 [Stgy->TPC]20130306 134715.229,[StrategyStatus],RecievedMarketData|[StrategyStatus],ActiveTicker:Futures,KOSPI,201303|[StrategyStatus],BarsFull1|[StrategyStatus],Processing Futures,KOSPI,201303|[StrategyStatus],Data Added|[StrategyStatus],Processing Futures,KOSPI,201306|[StrategyStatus],No Data|[StrategyStatus],CancelLimitOrders|[StrategyStatus],StartDecisionMaking|[StrategyStatus],ReadyToCalculate|[StrategyStatus],DecisionMaking2|[SendOrder],LimitOrder,2013-03-06 13:47:15.229,TraderName=Alpha,Id=64,TickerName=Futures_KOSPI_201303,BuySellType=Buy,Price=267.25,Quantity=1,Memo=
	//it's a st[sendOrder].
	list<Order* >ol;
	int iresult = 0;
	string strSep = "|";
	try{ 
		vector<string>  v1 = this->tc.split(qrcmsg,strSep, "<null>");
		for(int k=0; k<v1.size();k++)  
		{  
			if(k==0)
			{
				int tmpfirst = v1[k].find(orderSignal);
				if(tmpfirst >=0)
				{
					iresult++;
					ol.push_back(String2Order(v1[k].substr(tmpfirst)));
				}
			}
			else
			{
				if(v1[k].substr(0,orderSignal.size()).compare(orderSignal)==0)
				{
    				iresult++;
					ol.push_back(String2Order(v1[k]));
				}
			}
		} 
		/*step 2 QRC rtn msg status touch*/
		if(iresult>0)
		{
			//[sendOrder]
			this->thisQMstatus = 1;
		}
		else
		{
			//heart beat
			this->thisQMstatus = 0;
		}
		/*step 3 QRC order processing logical*/
		if(this->lastQMstatus == -1 || this->lastQMstatus == 0)
		{
			if(this->thisQMstatus == 0)
			{
				this->lastQMstatus = this->thisQMstatus;    
			}
			else if(this->thisQMstatus == 1)
			{
				placeQrcOrders_firstTime(ol);
				this->lastQMstatus =this->thisQMstatus;
			}
		}
		else if(this->lastQMstatus == 1)
		{
			if(this->thisQMstatus == 0)
			{
				this->myCancelAll();
				this->lastQMstatus =this->thisQMstatus;
			}
			else if(this->thisQMstatus == 1)
			{
				//handle all the order
				placeQrcOrders_again(ol);
				this->lastQMstatus =this->thisQMstatus;
			}
		}
	}  
	catch (CMemoryException* e)
	{
		cout<< "ProcessRtn CMemoryException" << endl;
	    return 0;
	}
	catch (CException* e)
	{
		cout<< "ProcessRtn CException" << endl;
		return 0;
	} 

	return iresult;
}
/*
* Author: Aaron 
* Date: 2013-3-13
* [sendOrder] message put into a order instance.
* example of string strSendOrder: [SendOrder],LimitOrder,2013-03-06 13:47:15.229,TraderName=Alpha,Id=64,TickerName=Futures_KOSPI_201303,BuySellType=Buy,Price=267.25,Quantity=1,Memo=
*/
Order*  QrcStrategy::String2Order(string strSendOrder)
{
	Order* order;
	string OrderType = "";
	string strSep = ",";

	string sectmp = ""; // sec name
	Stype typetmp ;     //direction
	double pricetmp = 0; //sendprice
	int lottmp = 0;  //lots

	try{ 
		vector<string>  vorder = this->tc.split(strSendOrder,strSep, "<null>");
		for(int k=0; k<vorder.size();k++)  
		{  
			//cout<<vorder[k].c_str()<<endl;  
			if(vorder[k].compare("LimitOrder") == 0)
			{
				OrderType = "LimitOrder";
			}
			else if(vorder[k].compare("MarketOrder") == 0)
			{
				OrderType = "MarketOrder";
			}
			else if(vorder[k].substr(0,10).compare("TickerName")==0)
			{
				sectmp = contractNameTrans(vorder[k].substr(11));
			}
			else if(vorder[k].substr(0,11).compare("BuySellType")==0)
			{
				string Direction = vorder[k].substr(12);
				if(Direction.compare("Buy")==0)
				{
					typetmp = Buy;
				}
				else if(Direction.compare("Sell")==0)
				{
					typetmp = Sellshort;
				}
			}
			else if(vorder[k].substr(0,5).compare("Price")==0)
			{
				string strPrice = vorder[k].substr(6);
				if(strPrice.compare("非数字")==0)
				{
					pricetmp = 0;
					cout<< "MarketOrder "<<endl;
				}
				else
				{
					pricetmp = tc.stringToDouble(strPrice);
				}
			}
			else if(vorder[k].substr(0,8).compare("Quantity")==0)
			{
				string strVol = vorder[k].substr(9);
				//cout<<strVol<<endl;
				lottmp = tc.stringToInt(strVol);
			}
		}
		order = new Order(this->strategyID, 0, 
			sectmp, pricetmp,typetmp, 0, lottmp, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
	}  
	catch (char *s) {  
		cout<<"QrcStrategy string2order"<<s<<endl;  
		return order;
	}   
	return order;
}
/*
* Author: Aaron 
* Date: 2013-3-13
* judge the type of qrc rtn msg. 
* return 0 for heart beat;1 for [sendOrder]
*/
int QrcStrategy::getQMStatus(string qmsg)
{
	return 1;
}
/*
* Author: Aaron 
* Date: 2013-3-13
* Function：place order for all the signal of one QRC rtn record(the last signal is heart beat,so it's a firstTime [sendOrder] signal).
* Example: have 2 long position, get a signal of sell 3,then first close all the long positon then open 1 lots short.
*/
int QrcStrategy::placeQrcOrders_firstTime(list<Order* > ol)
{
	list<Order*>::iterator it = ol.begin();
	int i =0;
	for(;it!=ol.end();it++)
	{
		Order* t = (*it);	
		int vol = t->Volume;
		Position* pos = ats->pGlobalDm->getPositionMana()->getPositionBysec(t->sec);
		int spvol = pos->getSellopen();
		int bpvol = pos->getBuyopen();
		if(t->getType() == Stype::Buy)
		{
			if(spvol <= 0)
			{
				t->orderID = ats->pGlobalDm->getOrdersMana()->getLastOrderID();
				t->setSendTime(ss.getTime());
				t->Date = ss.getDate();
				mySendOrder(t);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,buy,spvol<=0,"+ t->getString());
			}
			else if(spvol > 0 && spvol < vol)
			{
				//first close 
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buytocover, 0, spvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,buy,firstClose,"+ or->getString());

				Order* or1 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buy, 0, vol-spvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or1);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,buy,thenOpen,"+ or1->getString());
			}
			else if(spvol >= vol)
			{
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buytocover, 0, vol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,buy,spvol>=vol,"+ or->getString());
			}

		}
		else if(t->getType() == Stype::Sellshort) 
		{
			if(bpvol <= 0)
			{
				t->orderID = ats->pGlobalDm->getOrdersMana()->getLastOrderID();
				t->setSendTime(ss.getTime());
				t->Date = ss.getDate();
				mySendOrder(t);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,sell,spvol<=0,"+ t->getString());
			}
			else if(bpvol > 0 && bpvol < vol)
			{
				//first close 
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sell, 0, bpvol,ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,sell,firstClose,"+ or->getString());

				Order* or1 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sellshort, 0, vol-bpvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or1);
				ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,sell,thenOpen,"+ or1->getString());
			}
			else if(bpvol >= vol)
			{
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sell, 0, vol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				mySendOrder(or);
					ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_firstTime,sell,bpvol>=vol,"+ or->getString());
			}
		}
		i++;
	}            
	return 1;
}
/*
* Author: Aaron 
* Date: 2013-3-13
* Function：place order for all the signal of one QRC rtn record(the last signal is [sendOrder] signal too).
* Example: if the Orders you want to place is already been placed ,then do nothing;if the orders been placed have different price ,cancell all the present orders and place new ones. 
*/
int QrcStrategy::placeQrcOrders_again(list<Order* > ol)
{
	list<Order*>::iterator it = ol.begin();
	int i =0;
	for(;it!=ol.end();it++)
	{
		Order* t = (*it);	
		int vol = t->Volume;
		Position* pos = ats->pGlobalDm->getPositionMana()->getPositionBysec(t->sec);
		int spvol = pos->getSellopen();
		int bpvol = pos->getBuyopen();
		if(t->getType() == Stype::Buy)
		{
			if(spvol <= 0)
			{
				t->orderID = ats->pGlobalDm->getOrdersMana()->getLastOrderID();
				t->setSendTime(ss.getTime());
				t->Date = ss.getDate();
				judgeOrder(t);
			}
			else if(spvol > 0 && spvol < vol)
			{
				//first close 
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buytocover, 0, spvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or);

				Order* or1 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buy, 0, vol-spvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or1);
			}
			else if(spvol >= vol)
			{
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Buytocover, 0, vol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or);
			}

		}
		else if(t->getType() == Stype::Sellshort) 
		{
			if(bpvol <= 0)
			{
				t->orderID = ats->pGlobalDm->getOrdersMana()->getLastOrderID();
				t->setSendTime(ss.getTime());
				t->Date = ss.getDate();
				judgeOrder(t);
			}
			else if(bpvol > 0 && bpvol < vol)
			{
				//first close 
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sell, 0, bpvol,ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or);

				Order* or1 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sellshort, 0, vol-bpvol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or1);
			}
			else if(bpvol >= vol)
			{
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					t->sec, t->getSendPrice(),Sell, 0, vol, ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
				judgeOrder(or);
			}
		}
		i++;
	}            
	return 1;
}
/*
* Author: Aaron 
* Date: 2013-3-14
* Function：if the order is placing and the price is the same, do nothing; 
if the order is placing and the price is different,cancel the old order ,then place a new one with the new price.
if the order isn't existed,place this order.
if there are more than one order in history,cancel all ,placing the new one.
* Example: 
*/
int QrcStrategy::judgeOrder(Order * order)
{
	int irtn = 0;
	string con = order->sec;
	list<int> uoli = ats->pGlobalDm->getOrdersMana()->getUnTradeOrderID(con);

	if(uoli.size() == 0)
	{
		mySendOrder(order);
        ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_again,judgeOrder,uoli.size==0,"+ order->getString());
		irtn = 1;
	}
	else if(uoli.size()==1)
	{
		Order* lastOrder = ats->pGlobalDm->getOrdersMana()->getOrderByID(*uoli.begin());
		if((lastOrder->SendPrice == order->SendPrice)  && (lastOrder->Volume == order ->Volume))
		{
			//do nothing
			ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_again,judgeOrder,hold,"+ order->getString());
		}
		else
		{
			//pGlobalDm->getOrdersMana()->CancelOrder(*uoli.begin());
			this->myCancelSingle(*uoli.begin());
			mySendOrder(order);
			ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_again,judgeOrder,Amend,"+ order->getString());
		}
		irtn = 2;
	}
	else if(uoli.size() > 1)
	{
		//pGlobalDm->getOrdersMana()->CancelAll(con);
		this->myCancelAll(con);
		mySendOrder(order);
		ats->pGlobalDm->keepRecord(HisOrder, "placeQrcOrders_again,judgeOrder,CancelAllAmendEmerg,"+ order->getString());
		irtn = 3;
	}
	return irtn;
}
void QrcStrategy::mySendOrder(Order* order)
{
	if(isTradingReal)
	{
		/*

		Order*  or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
		Sec1, ss1.getLastPrice() + 2,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
		ats->cc->sendorder(or);
		*/
		//ats->cc->sendorder(order);	
		algomanager->sendorder(order,ats);
	}
	else 
	{
		stl->sendOrder(order);
	}
	//pGlobalDm->keepRecord(HisOrder, order->getString());

}
bool  QrcStrategy::myCancelAll()
{
	if(isTradingReal)
	{
		bool res = false;        
		list<Order*>::iterator it = ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().begin();
		int i = 0;
		for(;it!=ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().end();)
		{
			//ats->cc->cancelOrder(*it);
			algomanager->cancelOrder(*it,ats);
			ats->pGlobalDm->keepRecord(CancelOrders, "CancelAll(),"+tc.IntToString((*it)->getOrderID()));
			res = true;
			i++;
		}            
		cout<<"Cancel All,totally: "<<i<<endl;
		return res;
	}
	else
	{
		ats->pGlobalDm->getOrdersMana()->CancelAll();
		return true;
	}
}
bool  QrcStrategy::myCancelAll(string con)
{
	if(isTradingReal)
	{
		bool res = false;        
		list<Order*>::iterator it = ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().begin();
		int i = 0;
		for(;it!=ats->pGlobalDm->getOrdersMana()->getUnTradeOrder().end();)
		{
			if((*it)->getSec().compare(con)==0)
			{
				//ats->cc->cancelOrder(*it);
				algomanager->cancelOrder(*it,ats);
				ats->pGlobalDm->keepRecord(CancelOrders, "CancelAll(),"+tc.IntToString((*it)->getOrderID()));
				res = true;
				i++;
			}
		}            
		cout<<"Cancel All,totally: "<<i<<endl;
		return res;
	}
	else
	{
		ats->pGlobalDm->getOrdersMana()->CancelAll(con);
		return true;
	}

}
bool  QrcStrategy::myCancelSingle(int orID)
{
	bool res = false;
	if(isTradingReal)
	{
		//res = ats->cc->cancelOrder(pGlobalDm->getOrdersMana()->getOrderByID(orID));
		res = algomanager->cancelOrder(ats->pGlobalDm->getOrdersMana()->getOrderByID(orID),ats);
		ats->pGlobalDm->keepRecord(CancelOrders, "CancelSingle(),"+tc.IntToString(orID));
	}
	else
	{
		res = ats->pGlobalDm->getOrdersMana()->CancelOrder(orID);
		ats->pGlobalDm->keepRecord(CancelOrders, "CancelSingle(),"+tc.IntToString(orID));
	}
	return res;
}
/*
* Author: Aaron 
* Date: 2013-3-14
* wait a moment to develop this function
* Example: 
*/
int QrcStrategy::myPositionReport(string sec, int position)
{
	return 1;
}
/*
* Author: Aaron 
* Date: 2013-3-14
*  translate the contract names.
now we use a temp way.still need modify.
* Example: 
*/
string QrcStrategy::contractNameTrans(string qrccon)
{
	string result=qrccon;
	//Futures_CSI300_201304
	if(qrccon.size() >= 8)
	{
		string conHead = qrccon.substr(0,8);
		if(conHead.compare("Futures_")==0)
		{
			vector<string>  tmpv = this->tc.split(qrccon,"_", "<null>");
			if(tmpv.size()>=3)
			{
				if(ats->qpi->exchange.compare("CZCE")==0)
				{
					result = ats->pro1 + tmpv[2].substr(3); //ER003
				}
				else
				{
					result = ats->pro1 + tmpv[2].substr(2); //CU1003
				}

			}
		}
	}
	return result;
}

void QrcStrategy::UnitTest(int i)
{
	//string strsrc = "20130305 144000  [StrategyStatus],BarBasedSignal|[StrategyStatus],RecieveMarketData|[StrategyStatus],ActiveTicker:Futures,CSI300,201304|[StrategyStatus],BarsFull1|[StrategyStatus],CancelLimitOrders|[StrategyStatus],BarsFull2|[StrategyStatus],DecisionMaking3|[StrategyStatus],GotGoodBar2|[StrategyStatus],ReadyToCalculate|[StrategyStatus],DecisionMaking1|[StrategyStatus],DecisionMaking4|[StrategyStatus],GotGoodBar2|[StrategyStatus],ReadyToCalculate|[SendOrder],LimitOrder,2013-03-05 14:40:00,TraderName=Alpha,Id=1,TickerName=Futures_CSI300_201304,BuySellType=Buy,Price=2635,Quantity=1,Memo=";
	//ProcessQrcRtn(strsrc);
	/*
	if(i==1)
	{
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			"IF1304", ss.getLastPrice(),Buy, 0, 1,ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
		mySendOrder(or);
	}
	else if(i==2)
	{
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			"IF1304", ss.getLastPrice(),Sell, 0, 1,ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
		mySendOrder(or);
	}
	else if(i==3)
	{
		ats->pGlobalDm->getOrdersMana()->CancelAll();
	}
	else if(i==4)
	{
		ats->pGlobalDm->getOrdersMana()->CancelAll("IF1304");
	}
	else if(i==5)
	{
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			"IF1304", ss.getLastPrice(),Buy, 0, 1,ss.getDate(), ss.getTime(),ats->qpi->exchange,ats->pGlobalDm->getAccountMana(),tc.stringToDouble(ats->qpi->afcommision),tc.stringToDouble(ats->qpi->afmarginratio),tc.stringToInt(ats->qpi->pointValue));
		mySendOrder(or);


	}
	*/
}

int QrcStrategy::initReqtoQRC(double x)
{
	this->qrc->initReq(x);
	return 1;
}

int QrcStrategy::preFeedin()
{
	return this->qrc->preFeedin();

}
string QrcStrategy::dummyFeedin(BarValue bv,double position,bool isEnd)
{
	return this->qrc->BarFeedin(bv,position,isEnd);
}
//preLoad 与 preFeedin的区别，
/*
preFeedin,执行端在C#那边。用于快速preload数据。
preload函数，是考虑到可以在preload过程中进行发单逻辑测试，所以执行端放在C++了。
*/
int QrcStrategy::preLoad()
{
	try{
	int countresult = 0;
	char buffer[500]; 
	getcwd(buffer,   500);
	string roof = buffer;
	string fullPath = roof + "\\" + this->ats->qpi->tickername + "_" +  this->ats->qpi->port + "\\"+"*.csv";
	//cout<<fullPath<<endl;

	const char* name1 = fullPath.c_str();
	_finddata_t file;
	long lf;
	if((lf=_findfirst(name1,&file))==-1l)
	{
		cout<<"QrcStrategy::preLoad 该目录不存在！\n"<<endl;
	}
	else
	{
		while(_findnext(lf,&file)==0)
		{
			if(file.name[2]==0)
			{
				continue;
			}
			string singlefullname = roof + "\\"  + this->ats->qpi->tickername + "_" +  this->ats->qpi->port +"\\"+ file.name;
			cout<<singlefullname<<"\n";
			/*****single file process******************************/
			char buf[512]; //临时保存读取出来的文件内容
			ifstream infile;
			infile.open(singlefullname);
			string firststr = "";
			string secondstr = "";

			if(infile.is_open()) 
			{
				if(infile.good() && !infile.eof())
				{
					memset(buf,0,sizeof(buf));
					infile.getline(buf,sizeof(buf)); 
					firststr = buf ;
				}
				while(infile.good() && !infile.eof())
				{
					memset(buf,0,sizeof(buf));
					infile.getline(buf,sizeof(buf)); 
					string  secondstr = buf ;
					if(secondstr.length() < 1)
					{
						continue;
					}
					//cout<< firststr <<endl;
					vector<string> firstBar = this->tc.split(firststr,",","<null>");//firststr是空的
					vector<string> secondBar = this->tc.split(secondstr,",","<null>");
					if(firstBar[3].compare(secondBar[3])==0)
					{
						this->qrc->preLoad(firstBar[0] + "," + firstBar[1] + "," + firstBar[2],
							firstBar[3],firstBar[4],firstBar[5],firstBar[6],firstBar[7],firstBar[8],"0",false);
					
					}
					else
					{
						string preloadResult = this->qrc->preLoad(firstBar[0] + "," + firstBar[1] + "," + firstBar[2],
							firstBar[3],firstBar[4],firstBar[5],firstBar[6],firstBar[7],firstBar[8],"0",true);
						if(ispreloadTrade)
						{
							ProcessQrcRtn(preloadResult);
						}
					}
					firststr = secondstr;
					secondstr="";
				}
				if(firststr.length()>1)
				{
					vector<string> finalBar = this->tc.split(firststr,",", "<null>");
					string preloadResult =this->qrc->preLoad(finalBar[0] + "," + finalBar[1] + "," + finalBar[2],
						finalBar[3],finalBar[4],finalBar[5],finalBar[6],finalBar[7],finalBar[8],"0",true);
					if(ispreloadTrade)
					{
						ProcessQrcRtn(preloadResult);
					}
				}
			}
			infile.close();
			countresult ++;

			/*****single file process******************************/
			
		}
	}
	cout<<"file list ended"<<endl;
	_findclose(lf);
	return countresult;
	}
	catch (CMemoryException* e)
	{
		cout<< "CMemoryException" << endl;
		return 0;
	}
	catch (CException* e)
	{
		cout<<"preLoad exception"<<endl;
		return 0;
	}  
}
//like 091500->091600 or 105900->110000
string QrcStrategy::minutePP(string time)
{
	int intTime = atoi(time.c_str());
	if(intTime < 10000)
	{
		cout<< "QRCadapter::minutePP atoi fail:"+ time<<endl;
		return "["+time+"]";
	}
	else
	{
		int HH = intTime/10000;
		int MM = (intTime%10000)/100;
		if(MM==59)
		{
			HH++;
			MM =0;
		}
		else
		{
			MM++;
		}
		int finalTime = HH*10000 + MM * 100;
		char chartime[10];
		itoa(finalTime,chartime,10);
		string strtime = chartime;

		//092000
		if(strtime.length()==5)
		{
			string tmpzero = "0";
			strtime = tmpzero.append(strtime);
		}

		return strtime;
	}
}



