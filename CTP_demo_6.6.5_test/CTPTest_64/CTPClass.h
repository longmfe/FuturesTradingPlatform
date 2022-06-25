#pragma once
#include <string>
#include <map>
#include "Order.h"
#include "TickData.h"
//#include "AlgoTSEng.h"
//#include "TradeApplication.h"
#include "MarketApplication.h"
//#include "ExecReport.h"
//#include "KeepRecord.h"
#include "AlgoManager.h"

//#include "contractDetail.h"

class TradeApplication;
class MarketApplication;
using namespace std;
//class AlgoTSEng;
class AlgoManager;
class CTPClass
{
public:
	CTPClass(AlgoManager* agm1);
	//CTPClass(void){};
	~CTPClass(void);
private:
	TradeApplication* fixTrader;
    MarketApplication* fixMD;
	AlgoManager* agm;
public:
	 
    bool isTradeLogon ;
    bool isMDLogon;
    int orderRef;
 
    bool getIsLogonSuccess();
    bool getIsTradeLogon();
	void setIsTradeLogon(bool isOK);
	bool getIsMDLogon();
	void setIsMDLogon(bool isOK);
	void runCTP();
    
	void requestPosition();
	void  requestMD(string symbolname );
	
 
    bool sendorder(Order* order);

    bool cancelOrder(Order* order);
    
    //void dealExecutionReport( );
    
    
	TradeApplication* getCTPTrader();
 
    
	MarketApplication* getCTPMD();
 

	 
	void UpdateMarket(TickData tdtmp);
	void setOrderRef(int of);
	 

	//added by Aaron 2013-3-6
	void QryContractList();
   	list<string> FindContracts(string productName);
private:
	void Release();
	void login();
	//KeepRecord kp;
	map<int,Order*> orderMap;//1 orderref; 2 order pointor 
 
	//map<string,contractDetail*> conInfoMap;
public:
	 
	void writeLog(string parm);
	void OnRspInstrument(string intrumentName, int iflag);
	void requestInstrument(void);//�������к�Լ
	void OnRspPosition(string instrumentname, int dirction, int volumn, double avgprice, int iflag);
	//void OnOrderVolumnUpdate(ExecReport ertmp);
	void OnOrderStatusUpdate(int orderRef,int orderStatus);
 
	void requestAccount();//�����ʺ��ʽ�
	void OnRspAccount(double quanyi,double availablemoney);
    //contractDetail * getContractDetail(string str);
	///����ӿ�ֻ�ܸ�TraderSPi���ã���ΪTradeSpi����MDSPIǰ��ȡ��Լ��Ϣ��
	///������ʼ������С�䶯�۸�
	//void addContractDetail(string con, contractDetail * cond);
	void updateContractDetailByMD(string con, double UpperLimitPrice, double LowerLimitPrice, double lastPrice );
};

