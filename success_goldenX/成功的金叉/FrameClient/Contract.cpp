#include "StdAfx.h"
#include "Contract.h"


Contract::Contract(void)
{

	isNULL = false;
	 //合约ID
    conid = "N/A";
    //交易所
     exchange = "N/A";
    //最小变化单位
     minPrice = 10.0;
    //手续费率
     commissionRate = 10.0;
    //保证金率
     ratioOfmargin = 0.1;
    //合约乘数
    multy = 10;
    //昨结
     preSettlement = 0.0;
    //开仓量
      openint = 0.0;
    //交易量
    tradeVol = 0.0;

}
//ID,最小变动（minMov），合约乘数（point value），手续费，保证金
Contract::Contract(string id, double minp, int mul, double comm, double ratio)
{
	    //合约ID
    conid = "N/A";
    //交易所
     exchange = "N/A";
    //最小变化单位
     minPrice = 10.0;
    //手续费率
     commissionRate = 10.0;
    //保证金率
     ratioOfmargin = 0.1;
    //合约乘数
    multy = 10;
    //昨结
     preSettlement = 0.0;
    //开仓量
      openint = 0.0;
    //交易量
    tradeVol = 0.0;
	 multy = mul;
        conid = id;
        minPrice = minp;
        commissionRate = comm;
        ratioOfmargin =ratio;

}

Contract::~Contract(void)
{
}
 
void Contract::setMinPrice(double value){
    minPrice = value;
}


    string Contract::getConid(){
    return conid;
}

double Contract::getMinPrice(){
    return minPrice;
}

void Contract::setCommissionRate(double value){
    commissionRate =value;
}

double Contract::getCommissionRate(){
    return commissionRate;
}

void Contract::setRatioOfmargin(double value){
    ratioOfmargin = value;
}

double Contract::getRatioOfmargin(){
    return ratioOfmargin;
}

void Contract::setMulty(int value){
    multy = value;
}

double Contract::getMulty(){
    return multy;
}
	
