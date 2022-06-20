#pragma once
#include <string>
#include <list>
#include "CTPClass.h"
//#include "ThostTraderApi\ThostFtdcMdApi.h"
//#include "ThostTraderApi\ThostFtdcTraderApi.h"
//#include "MdSpi.h"
#include <map>
#include "TickData.h"
//#include "KeepRecord.h"

using namespace std;
class CTPClass;
class CMdSpi;
class CThostFtdcMdApi;
struct RegMdStruct
{
	CThostFtdcMdApi* pUserApi;
	CMdSpi* pUserSpi;
	string FRONT_ADDRM;
};
class MarketApplication
{
public:
	MarketApplication(void);
	~MarketApplication(void);
private:
	CTPClass* fc;
    int timeOffset;
    bool bCheck ;
    string currentDate ;
    map<string,string> symbolList;
	CThostFtdcMdApi* pUserMdApi;
	CMdSpi* pUserMdSpi;
public:
	void checkDate(string date1);
	//void setFC(CTPClass* fc);

	void initial();

	string getTransTime(string time);

	string IntTotime(int IntTime) ;
private:
	int timeToInt(string time);
	//KeepRecord kc;
public:
	string getTranTime(string tt);
    //int sessiontype ; //0-trade 1-marketdata

	void login();

	void logout();
	//void SubcribeMarketDataSingle(string str);
	void SubcribeMarketData(string symbolname);
    void reRequestMarketData();
	void setLogonStatus(bool sta);
	bool getLogonStatus();
	void onMarketdata(TickData td);
	void Release();
private:
	string username;
	string password;
	string addrMD;
	string brokerID;
	RegMdStruct* rm;
	bool isLogon;
	
public:
	void writeLog(string parm);
	void reConnectForNewDay(void);
	void updateContractDetail(string con, double UpperLimitPrice, double LowerLimitPrice, double lastPrice);
};

