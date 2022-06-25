#include "StdAfx.h"
#include "TradeApplication.h"
#include "UtilFun.h"
#include "KeepRecord.h"
TradeApplication* ta= nullptr;
UtilFun uf1;

UINT RegistTradeThreadFunc(LPVOID lp)
{	
	if(ta!=NULL)
	{
		ta->writeLog("GoInto RegistTradeThreadFunc");
	}
	cout<<"RegistTradeThreadFunc run"<<endl;
	RegTradeStruct * m = (RegTradeStruct*)lp;
	m->pUserApi->RegisterSpi((CThostFtdcTraderSpi*)m->pUserSpi);		// 注册事件类
	m->pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// 注册公有流
	m->pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// 注册私有流
	char tmp[100] ={0};
	strcpy(tmp,m->FRONT_ADDR.c_str());
	m->pUserApi->RegisterFront(tmp);// connect 
	m->pUserApi->Init();
	m->pUserApi->Join(); 
	if(ta!=NULL)
	{
		ta->writeLog("Exit RegistTradeThreadFunc");
	}
	return 0;
}
TIMERPROC   FooTimerFunAts1(   HWND,   UINT,   UINT,   DWORD   )
{
    //UtilFun uf;
	int hhmm = atoi(uf1.getCHCurDate().substr(8, 4).c_str());
    toolsClass tc;
	string tmp = tc.IntToString(hhmm);           
    if(uf1.IsWeekend()){
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":IsWeekend");
		}
        return 0;
    }
	
	//if(hhmm>840&&hhmm<842)// for real
	if(hhmm>1507&&hhmm<1509)// for test
	{
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":reach reconnect time");
		}
		ta->reConnectForNewDay();
	}else
	{
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":Not reach reconnect time");
		}
	}
               
    //if(!fc.isMDLogon){
	          
    /*if(!ats->bDone1 && hhmm > 845 && hhmm < 1510){
        ats->DoInitWork();
        ats->bDone1 = true;
        ats->bDone2 = false;
    }
                
    if(!ats->bDone2 && hhmm > 1515 && hhmm < 1525){
        ats->DoEndWork();
        ats->bDone2 = true;
        ats->bDone1 = false;
    }*/

	return 0;
}
TradeApplication::TradeApplication(void):kc("TradeSessionRecord")
{
	isLogon =false;
	readProperty rp;
	username=rp.getUserName();
	password=rp.getPassword();
	addrtrade=rp.getTradeLink();
	brokerID=rp.getbrokerID();
	rs=NULL;
	pUserTradeApi = NULL;
	pUserTradeSpi = NULL;
	ta = this;
	SetTimer(   NULL, 10001, 1000*60*1 ,   (TIMERPROC)FooTimerFunAts1 ); 
}

TradeApplication::~TradeApplication(void)
{
	if(pUserTradeSpi!=NULL)
	{
		delete pUserTradeSpi;
		pUserTradeSpi =NULL;
	}
	if(rs!=NULL)
	{
		delete rs;
		rs = NULL;
	}
}
void TradeApplication::setFC(CTPClass* fc)
{
    this->fc = fc;
} 

void TradeApplication::login(){

	pUserTradeApi= CThostFtdcTraderApi::CreateFtdcTraderApi();
	char bID[11]={0};
	strcpy( bID, brokerID.c_str() ); 
	pUserTradeSpi = new CTraderSpi(bID,pUserTradeApi,this);
	pUserTradeSpi->setPassword(password);
	pUserTradeSpi->setUserName(username);
	rs = new RegTradeStruct;
	rs->pUserApi = pUserTradeApi;
	rs->pUserSpi = pUserTradeSpi;
	rs->FRONT_ADDR = this->addrtrade;
	AfxBeginThread(RegistTradeThreadFunc,rs);
} 

void TradeApplication::logout(){
} 

  
void TradeApplication::sendNewOrderSingle(Order* order){

	this->pUserTradeSpi->sendOrder(*order);

} 
	 

void TradeApplication::requestPosition() {
	this->pUserTradeSpi->ReqQryInvestorPosition();
} 
    
void TradeApplication::cancelOrder(Order* order) {

	this->pUserTradeSpi->CancelOrder(*order);

} 

void TradeApplication::OrderStatusRequest(){

} 
    

 
void TradeApplication::sendSecurityListRequest() {
	this->pUserTradeSpi->requestInstrument();

} 
void TradeApplication::sendSecurityStatusRequest(string symbol){

} 
 
void TradeApplication::setLogonStatus(bool t){
	isLogon = t;
	toolsClass tc;
	int hhmm = atoi(uf1.getCHCurDate().substr(8, 4).c_str());
	string tmp = tc.IntToString(hhmm);   
	if(t==true)
	{
		this->writeLog(tmp+":TradeApplication login success");
	
	}else
	{
		this->writeLog(tmp+":TradeApplication login fail");
	}
	this->fc->setIsTradeLogon(t);
} 
void TradeApplication::Release(){
	pUserTradeApi->Release();
} 
void TradeApplication::setOrderRef(int orderref)
{
	this->fc->setOrderRef(orderref);
}


void TradeApplication::updateOrderStatus(int orderRef1, int istatus)
{
	this->fc->OnOrderStatusUpdate(orderRef1,istatus);
}


void TradeApplication::updateOrderTrade(ExecReport ec)
{
	this->fc->OnOrderVolumnUpdate(ec);
}


void TradeApplication::OnRspPosition(string instrumentname, int dirction, int volumn, double avgprice,int iflag)
{
	//this->fc->addPosition(er);
	this->fc->OnRspPosition(instrumentname,dirction,volumn,avgprice,iflag);
}


void TradeApplication::reConnectForNewDay(void)
{
	this->Release();
	isLogon =false;
	if(pUserTradeSpi!=NULL)
	{
		delete pUserTradeSpi;
		pUserTradeSpi =NULL;
	}
	if(rs!=NULL)
	{
		delete rs;
		rs = NULL;
	}
	this->login();
}


void TradeApplication::writeLog(string parm)
{
	kc.writeLine(parm);
}
list<string> TradeApplication::screenContracts(string productName)
{
	return this->pUserTradeSpi->screenContracts(productName);
}
void TradeApplication::OnRspAccount(double quanyi,double availablemoney)
{
	this->fc->OnRspAccount(quanyi,availablemoney);

}

void TradeApplication::requestAccount(void)
{
	this->pUserTradeSpi->ReqQryTradingAccount();
}
void  TradeApplication::OnRspInstrument(string intrumentName, int iflag)
{
	this->fc->OnRspInstrument(intrumentName,iflag);
}


void TradeApplication::addContractDetail(string con, contractDetail* cond)
{
	this->fc->addContractDetail(con,cond);
}
