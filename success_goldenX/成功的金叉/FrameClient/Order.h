#pragma once
#include <string>
#include "GlobalDataManage.h"
#include "toolsClass.h"
#include "AccountManage.h"
using namespace std;

enum Stype{
    Buy , Sell, Sellshort, Buytocover,Cancel
};

class Order
{
public: 
	Order(void);
	~Order(void);

	string StrategyName;
    int orderRef;// added for CTP order ref
    int orderID;
    string sec;
    
    //��������
    Stype type ;
    //�����۸�
    double SendPrice;
    //�ɽ�����
    double DealedAvgPrice;
    //���ۣ�����ʾ��Ϊ׼
    double SlipPrice;
    //�ɽ���
    double tradePrice;
    //������ÿ��λ
    double CommisionP;
    //��֤��ÿ��λ
    double MarginP;
    int Volume;
    //if wanna to support multiply trading, it can be List<int>
    int TradedVol;
    int TradeVolThisTime;

    string SendTime;
    string TradeAllTime;
    string Date;
	bool isNULL;
	int status;
	string exchangeName;

private:
	string split;
	AccountManage* m_am;
public:
	Order(string strategyname, int id, string s, double price, Stype t, double slip, int vol, string date, string time,string exname, AccountManage* am);

	Order(string strategyname, int id, string s, double price, Stype t, double slip, int vol, string date, string time,string exname, AccountManage* am,double commsionrate,double marginrate,double multy);


    string getStrategyID();
    
    double getCommision();

    double getMargin();

    double getFroneCommision();

    double getFroneMargin();

    int getOrderID();

    string getSec();

    Stype getType();

    int getStatus();
   static const int Default = 0;
   static const int NewSigleOrderOk = 1;
   static const int New = 2;
   static const int PartialFill = 3;
   static const int Cancelled = 4;
   static const int Filled = 5;
   static const int Reject = 6;
    

    int getTradeVolume();

    void TradeOccur(double value, int vol);
    int getTradeVolThisTime();

    double getTradePrice();

    double getSendPrice();

    int getSendVol();

    string getString();

    string getTradeString();

    void setTradeTime(string value);

    string getSendTime();

     double getSlip();
	 void setStatus(int value);

	inline bool operator == (const Order &or1){
		return this->orderID == or1.orderID;
	}
	string getTradeTime();
	void setSendTime(string value);
	void setSlip(double value);
};

