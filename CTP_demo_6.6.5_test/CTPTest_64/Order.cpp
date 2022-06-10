#include "StdAfx.h"
#include "Order.h"
#include "AccountManage.h"
using namespace std;

//extern ContractsManage* g_conM;

Order::Order(void) :type(Stype::Buy), 
    //报单价格 
    CommisionP(0.0),
    //保证金每单位
    MarginP(0.0),
    split("---")
{
    isNULL = false;
    status = Default;
    SendPrice = 0;
    
    //成交均价
    DealedAvgPrice = 0;
    
    //滑价，以显示的为准
    SlipPrice = 0;
    
    //成交价
    tradePrice = 0;
    
    //手续费每单位
    CommisionP = 0;
    
    //保证金每单位
    MarginP = 0;
    
    Volume = 0;
    
    //if wanna to support multiply trading, it can be List<int>
    TradedVol = 0;
    TradeVolThisTime = 0;
}

Order::~Order(void)
{
}

Order::Order(string strategyname, 
        int id, string s, double price, 
        Stype t, double slip, int vol, 
        string date, string time,
        string exname, AccountManage* am)
    :type(Stype::Buy),
    //报单价格
    CommisionP(0.0),
    //保证金每单位
    MarginP(0.0),
    split(",")
{
    m_am = am; //account manage
    status = Default;
    SendPrice =0 ;
    
    //成交均价
    DealedAvgPrice=0 ;
    
    //滑价，以显示的为准
    SlipPrice=0 ;
    
    //成交价
    tradePrice =0 ;
    
    //手续费每单位
    CommisionP =0 ;
    
    //保证金每单位
    MarginP=0 ;
    Volume=0 ;
    
    //if wanna to support multiply trading, it can be List<int>
    TradedVol=0 ;
    TradeVolThisTime =0;
    
    // reassign variable
    StrategyName = strategyname;
    Date = date;
    orderID = id;
    SendPrice = price;          
    SlipPrice = slip;           
    Volume = vol;               
    SendTime = time;
    type = t;
    sec = s;

    //  CommisionP = g_conM->getContractByID(s)->getCommissionRate() * (g_conM->getContractByID(s)->getMulty());
    //  MarginP = g_conM->getContractByID(s)->getRatioOfmargin() * (g_conM->getContractByID(s)->getMulty());

    if(t == Stype::Buytocover || t == Stype::Sell){
            MarginP = 0;
    }

    this->exchangeName = exname;
}

Order::Order(string strategyname, int id, string s, 
        double price, Stype t, double slip, int vol, 
        string date, string time, string exname, 
        AccountManage* am, double commsionrate,
        double marginrate, double multy)
        :type(Stype::Buy),
        //报单价格
        CommisionP(0.0),
        //保证金每单位
        MarginP(0.0),
        split(",")
{
        m_am = am;
        status = Default;
        SendPrice =0 ;
        //成交均价
        DealedAvgPrice=0 ;
        //滑价，以显示的为准
        SlipPrice=0 ;
        //成交价
        tradePrice =0 ;
        //手续费每单位
        CommisionP =0 ;
        //保证金每单位
        MarginP=0 ;
        Volume=0 ;
        //if wanna to support multiply trading, it can be List<int>
        TradedVol=0 ;
        TradeVolThisTime =0;
        
        // reassign var
        StrategyName = strategyname;
        Date = date;
        orderID = id;
        SendPrice = price;              
        SlipPrice = slip;               
        Volume = vol;           
        SendTime = time;
        type = t;
        sec = s;
        CommisionP = commsionrate * multy;
        MarginP = marginrate *multy;

        if(t == Stype::Buytocover || t == Stype::Sell){
                MarginP = 0;
        }

        this->exchangeName = exname;
}


string Order::getStrategyID()
{
        return StrategyName;
}

double Order::getCommision()
{
        if(CommisionP < 1){
            return CommisionP * tradePrice * TradedVol;
        }
        else{
            return CommisionP*TradedVol;
        }
}

double Order::getMargin()
{
        double tt = MarginP* tradePrice * TradedVol;
        return  tt;
}

double Order::getFroneCommision()
{
        if(CommisionP < 1){
            return CommisionP * SendPrice * (Volume - TradedVol);
        }
        else{
            return CommisionP * (Volume - TradedVol);
        }
}

double  Order::getFroneMargin()
{
        return MarginP * SendPrice * (Volume - TradedVol);
}

int Order::getOrderID()
{
        return orderID;
}

string Order::getSec()
{
        return sec;
}

Stype Order::getType()
{
        return type;
}

int  Order::getStatus()
{
       return status;
}

int Order::getTradeVolume()
{
        return TradedVol;
}

void Order::TradeOccur(double value, int vol){
        tradePrice = value;
        //effect the account's fronecomin, fronemagin, comin, margin, 
        AccountManage* am = m_am;

        double tmp2 = am->getFroneComm();
        double tmp3 = am->getFronemargin();
        double tmp4 = am->getCommision();
        double tmp5 = am->getMargin();

        double tmp = 0;

        // calc commission
        if(CommisionP < 1){
            tmp = tmp2 - vol * SendPrice * CommisionP;
            am->addFroneCommision(- vol * SendPrice * CommisionP);
        }
        else{
            tmp = tmp2 - vol * CommisionP;
            am->addFroneCommision(- vol * CommisionP);
        }
        //am.setFroneComm(tmp);
        
        // calc margin
        tmp = tmp3 - MarginP * SendPrice * vol;
        //am.setFronemargin(tmp);
        am->addFroneMargin(- MarginP * SendPrice * vol);

        if(CommisionP < 1){
            tmp = tmp4 + vol * tradePrice * CommisionP;
            am->addCommision(vol * tradePrice * CommisionP);
        }
        else{
            tmp = tmp4 + vol * CommisionP;
            am->addCommision(vol * CommisionP);
        }
        //am.setCommision(tmp);
        //tmp = tmp5 + MarginP * tradePrice * vol;
        //am.setMargin(tmp);
        am->addMargin(MarginP * tradePrice * vol);

        TradedVol += vol;

        string str = ""; // TODO: for what
        string di = ""; // direction

        if(type == Stype::Buy){
                di = "buyopen";
        }
        else if(type == Stype::Buytocover){
                di = "buytocover";
        }
        else if(type == Stype::Sell){
                di = "sellcover";
        }
        else if(type == Stype::Sellshort){
                di = "sellshort";
        }
        
        toolsClass tc;
        str = "Strategy ID:" + StrategyName 
        + split + "Sec:" + sec 
        + split + "Direction:" + di 
        + split + "Price:" + tc.doubleToString(value) 
        + split + "lots" + tc.doubleToString(vol) 
        + split + "TradeTime:" + TradeAllTime 
        + ","   + tc.doubleToString(am->getEquity()) 
        + ","   + tc.doubleToString(am->getFreeMoney());
  
        TradeVolThisTime = vol;
}

string Order::getTradeTime()
{
    return TradeAllTime ;
}

string Order::getSendTime()
{
    return SendTime;
}

void Order::setStatus(int value)
{
    status = value;
}

void Order::setTradeTime(string value)
{
    TradeAllTime = value;
}
int Order::getTradeVolThisTime()
{
    return TradeVolThisTime;
 }

double Order::getTradePrice()
{
    return tradePrice;
}

double Order::getSendPrice()
{
    return SendPrice;
}

int Order::getSendVol()
{
    return Volume;
}

string Order::getString()
{
        string str = "";
        string di = "";
        if(type == Stype::Buy){
                di = "buyopen";
        }
        else if(type == Stype::Buytocover){
                di = "buytocover";
        }
        else if(type == Stype::Sell){
                di = "sellcover";
        }
        else if(type == Stype::Sellshort){
                di = "sellshort";
        }
        
        toolsClass tc ;
        //str = "Id:" + tc.IntToString(orderID) + split + "Sec:" + sec + split + "Direction:" + di + split + "Price:" + tc.doubleToString(SendPrice)
        ///     + split
        //         + "lots" + tc.doubleToString(Volume) + split + "Time:" + SendTime + split + "Date:" + Date;
        str = this->Date + split + 
        this->SendTime + split + 
        tc.doubleToString(orderID) + split + 
        this->sec + split +
        di + split +
        tc.doubleToString(this->SendPrice) + split +
        tc.doubleToString(this->Volume);
        return str;
}

string Order::getTradeString()
{
        string str = "";
        string di = "";
        if(type == Stype::Buy){
                di = "buyopen";
        }
        else if(type == Stype::Buytocover){
                di = "buytocover";
        }
        else if(type == Stype::Sell){
                di = "sellcover";
        }
        else if(type == Stype::Sellshort){
                di = "sellshort";
        }

        toolsClass tc;
        str = "StrategyName" + StrategyName 
        + split + "Sec:" + sec 
        + split + "Direction:" + di 
        + split + "Price:" + tc.doubleToString(SendPrice) 
        + split + "lots" + tc.doubleToString(Volume) 
        + split + "Date" + Date 
        + split + "TradeTime:" + TradeAllTime;
        return str;
}

void Order::setSendTime(string value)
{
            SendTime = value;
}

double Order::getSlip()
{
        return SlipPrice;
}

void  Order::setSlip(double value)
{
        SlipPrice = value;
 }