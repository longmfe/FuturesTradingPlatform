#pragma once
#include "StdAfx.h"
#include "CTPClass.h"
#include <list>
#include "DataManager.h"

using namespace std;
//extern GlobalDataManage* pGlobalDm;

CTPClass::CTPClass(AlgoManager* agm1)//:
	//kp("CTPClassLogInfo")
{
	isTradeLogon = false;
	isMDLogon = false;
	this->fixMD = new MarketApplication() ;
	this->fixMD->setFC(this);
	//this->fixTrader =new TradeApplication() ;
	//this->fixTrader->setFC(this);
	orderRef = 0;
    agm =agm1;
}

void CTPClass::UpdateMarket(TickData tdtmp){
	cout << "CTPClass::UpdateMarket" << endl;
	DataManager* dm = new DataManager();
	dm->recordMarketData(&tdtmp);
	//algotseng->process(tdtmp); no suck fuction in AlgoTSEng 
	// add bypass fucntion
	//void process(string con, double lastp, double lastV, double askp, double askV,
	//	         double bidP, double bidV, double openI, double traddV, string date
	//			 ,string time);

	/*algotseng->process(tdtmp.getSec(),tdtmp.getLastPrice(),tdtmp.getLastVol(),tdtmp.getAskPrice(),
		tdtmp.getAskVol(),tdtmp.getBidPrice(),tdtmp.getBidVol(),
		tdtmp.getOpenInt(),tdtmp.getTradeVol(),tdtmp.getDate(),tdtmp.getTime());*/
	string con = tdtmp.sec;
	double lastp = tdtmp.getLastPrice();
	double lastV = tdtmp.getLastVol();
	double askP = tdtmp.getAskPrice();
	double askV = tdtmp.getAskVol();
	double bidP = tdtmp.getBidPrice();
	double bidV = tdtmp.getBidVol();
	double openI  =tdtmp.getOpenInt();
	double traddV = tdtmp.getTradeVol();
	string date = tdtmp.getDate();
	string time = tdtmp.getTime();
	//algotseng->process(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
	//should add the following fucntion
	// agm->process(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
		//void OnMarketData(string con, double lastp, double lastV, double askp, double askV,
		//         double bidP, double bidV, double openI, double traddV, string date
		//		 ,string time);
	agm->OnMarketData(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
}
