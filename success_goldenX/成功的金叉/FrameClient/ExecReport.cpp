#include "StdAfx.h"
#include "ExecReport.h"


ExecReport::ExecReport(void)
{
	   type = Stype::Buy;
	   tradetime = "";
	   split = ",";
	   orderRef= 0;
	   TradeVol= 0;
	   sec ="";
	   tradePrice= 0;
	   orderID= -1;
}


ExecReport::~ExecReport(void)
{
}
string ExecReport::getTradetime(){
	return tradetime;
}
void ExecReport::setTradetime(string tradetime) {
	this->tradetime = tradetime;
}

 
/**
    * @return Returns the orderID.
    */
int ExecReport::getOrderID() 
{
	return orderID;
}
/**
    * @param orderID The orderID to set.
    */
void ExecReport::setOrderID(int orderID){

	this->orderID = orderID;
}

/**
    * @return Returns the sec.
    */
string ExecReport::getSec(){
	 return sec;

}
/**
    * @param sec The sec to set.
    */
void ExecReport::setSec(string sec){

	 this->sec = sec;
} 
/**
    * @return Returns the tradePrice.
    */
double ExecReport::getTradePrice(){
	  return tradePrice;
} 
/**
    * @param tradePrice The tradePrice to set.
    */
void ExecReport::setTradePrice(double tradePrice){
	  this->tradePrice = tradePrice;
} 
/**
    * @return Returns the tradeVol.
    */
int ExecReport::getTradeVol(){
	 return TradeVol;
} 
/**
    * @param tradeVol The tradeVol to set.
    */
void ExecReport::setTradeVol(int tradeVol) {
	 TradeVol = tradeVol;
} 
    
    
/**
    * @return Returns the Order type.
    */
Stype ExecReport::getType() {

	 return type;
} 
/**
    * @param tradeVol The Order type. to set.
    */
void ExecReport::setType( Stype value){
	 type = value;
} 
    
string ExecReport::toString(){
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
	//str = "Id:" + orderID + split + "Sec:" + sec + split + "Direction:" + di + split + "Price:" + tc.doubleToString(tradePrice)
	//	+ split + "lots" + tc.doubleToString(TradeVol) + split + "Time:" + tradetime;
	str = sec + split + di + split + tc.doubleToString(tradePrice) + split + tc.doubleToString(TradeVol) + split + tradetime ;
    return str;

	// in order to match the backtesting report ,format the str.
} 
