#pragma once
#include <string>
using namespace std;
class Contract
{
public:
	Contract(void);
	~Contract(void);
	bool isNULL;
private:
	//合约ID
    string conid ;
    //交易所
    string exchange;
    //最小变化单位
    double minPrice ;
    //手续费率
    double commissionRate;
    //保证金率
    double ratioOfmargin;
    //合约乘数
    int multy ;
    //昨结
    double preSettlement;
    //开仓量
    double openint ;
    //交易量
    double tradeVol ;
public:
    Contract(string id, double minp, int mul, double comm, double ratio);

   void setMinPrice(double value);


    string getConid();

    double getMinPrice();

    void setCommissionRate(double value);

    double getCommissionRate();

    void setRatioOfmargin(double value);

    double getRatioOfmargin();

    void setMulty(int value);

     double getMulty();
};

