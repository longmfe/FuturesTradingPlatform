#pragma once
#include <string>
#include "Order.h"
#include "GlobalDataManage.h"
#include "ExecReport.h"
#include "AccountManage.h"

using namespace std;
class Order;
class ExecReport;
class GlobalDataManage;

class Position
{
public:
	
	Position(GlobalDataManage*);
	Position(Order or, GlobalDataManage*);
    Position(string s, GlobalDataManage*);   
	~Position(void);
	 
	 //合约ID
    string sec;
    //卖开仓位
    int sellopen;
    //买开仓位
    int buyopen ;
    //卖均价
    double sellAvg ;
    //买均价
    double buyAvg ;
    //浮动盈亏
    double floatProfit ;
    //买盈亏
    double BuyProfit  ;
    //卖盈亏
    double SellProfit ;
    //卖开保证金
    double sellMagin ;
    //买开保证金
    double buyMagin ;

    double sellEntryPrice ;
    double buyEntryPrice  ;

    double getSellEntryPrice();
    
    double getBuyEntryPrice();
    
    void addPosition(Order or);
    int getSellopen();

    int getBuyopen();

    double getBuyAvg();

    double getSellAvg();

    string getSec();

    double getFloatProfit();

    double getBuyProfit();

    double getSellProfit();

    void computeProfit(double price);
public:
	bool isNULL;
	void addExecReport(ExecReport er);
private:
	GlobalDataManage* m_am;
};

