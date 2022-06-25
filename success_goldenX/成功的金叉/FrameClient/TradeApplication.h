#pragma once
#include <string>
#include "CTPClass.h"
#include "ThostTraderApi\ThostFtdcMdApi.h"
#include "ThostTraderApi\ThostFtdcTraderApi.h"
#include "TraderSpi.h"
#include "readProperty.h"
#include "keeprecord.h"
using namespace std;
class CTPClass;
class CThostFtdcTraderApi;
class CTraderSpi;
class contractDetail;
struct RegTradeStruct
{
	CThostFtdcTraderApi* pUserApi;
	CTraderSpi* pUserSpi;
	string FRONT_ADDR;
};

class TradeApplication
{
private:
	CTPClass* fc;
	bool isLogon;
	CThostFtdcTraderApi* pUserTradeApi;
	CTraderSpi* pUserTradeSpi;
	string username;
	string password;
	string addrtrade;
	string brokerID;
	RegTradeStruct* rs;

public:

	TradeApplication(void);
	~TradeApplication(void);
	//private FIXClass fc = new FIXClass();
    void setFC(CTPClass* fc) ;
	void login() ;
	void logout() ;

	void sendNewOrderSingle(Order* order) ;
	 

	void requestPosition() ;
    
	void cancelOrder(Order* order) ;

	void OrderStatusRequest() ;
    

	void sendSecurityListRequest() ;
	void sendSecurityStatusRequest(string symbol) ;
	 
	void setLogonStatus(bool t) ;
	void Release();
	void setOrderRef(int orderref);
	bool getLogonStatus(){return this->isLogon;};
	void updateOrderStatus(int orderRef1, int istatus);
	void updateOrderTrade(ExecReport ec);
	void OnRspPosition(string instrumentname, int dirction, int volumn, double avgprice,int iflag);
	void reConnectForNewDay(void);
	void writeLog(string parm);
	void OnRspAccount(double quanyi,double availablemoney);
	KeepRecord kc;
	//added by Aaron 2013-3-7
	list<string> screenContracts(string productName);
	void requestAccount(void);
// iflag = 0 not the last; iflag =1 the last
    void  OnRspInstrument(string intrumentName, int iflag);
	void addContractDetail(string con, contractDetail* cond);
};

