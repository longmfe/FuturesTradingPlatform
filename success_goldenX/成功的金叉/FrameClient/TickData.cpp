#include "StdAfx.h"
#include "TickData.h"
#include "toolsClass.h"

TickData::TickData(void)
{
	isNULL = false;
	sec = "N/A";
	exchange = "N/A";
	time = "N/A";
	date = "N/A";
	
	AskPrice = 0.0;
	AskVol = 0.0;
	BidPrice = 0.0;
	BidVol = 0.0;
	//if lastprice=null,lastprice=presettlement
	LastPrice = 0.0;
	LastVol = 0.0;
	
	PreSettlement = 0.0;
        
	OpenInt = 0.0;
	TradeVol = 0.0;
    separator = "_";
}


TickData::~TickData(void)
{
}
string TickData:: getSec()
{
	return sec;
}
TickData::TickData(string name)
{
	isNULL = false;
	sec = "N/A";
	exchange = "N/A";
	time = "N/A";
	date = "N/A";
	
	AskPrice = 0.0;
	AskVol = 0.0;
	BidPrice = 0.0;
	BidVol = 0.0;
	//if lastprice=null,lastprice=presettlement
	LastPrice = 0.0;
	LastVol = 0.0;
	
	PreSettlement = 0.0;
        
	OpenInt = 0.0;
	TradeVol = 0.0;
	sec = name;
}

void TickData::setAskPrice(double value){
	AskPrice = value;
}
	
double TickData::getAskPrice(){
	return AskPrice;
}
	
void TickData::setAskVol(double value){
	AskVol = value;
}
	
double TickData::getAskVol(){
	return AskVol;
}
void TickData::setBidPrice(double value){
	BidPrice = value;
}
	
double TickData::getBidPrice(){
	return BidPrice;
}
	
void TickData::setBidVol(double value){
	BidVol = value;
}
	
double TickData::getBidVol(){
	return BidVol;
}
	
void TickData::setLastPrice(double value){
	LastPrice = value;
}
	
double TickData::getLastPrice(){
	return LastPrice;
}
	
void TickData::setLastVol(double value){
	LastVol = value;
}
	
double TickData::getLastVol(){
	return LastVol;
}
	
void TickData::setOpenInt(double value){
	OpenInt = value;
}
	
double TickData::getOpenInt(){
	return OpenInt;
}
	
void TickData::setTradeVol(double value){
	TradeVol = value;
}
	
double TickData::getTradeVol(){
	return TradeVol;
}
	
void TickData::setTime(string value){
	time = value;
}
	
string TickData::getTime(){
	return time;
}
void TickData::setPreSettlement(double value){
	PreSettlement = value;
}
	
double TickData::getPreSettlement(){
	return PreSettlement;
}
        
void TickData::setDate(string value){
date = value;
}
	
string TickData::getDate(){
	return date;
}
string TickData::Object2String()
{
	toolsClass tc;
	return 
	sec + separator + date+ separator+ time +separator
		+ tc.doubleToString(LastPrice);
}

void TickData::fill(TickData ss)
{
	sec = ss.sec;
	exchange = ss.exchange;
	time = ss.time;
	date = ss.date;
	
	AskPrice = ss.AskPrice;
	AskVol = ss.AskVol;
	BidPrice = ss.BidPrice;
	BidVol = ss.BidVol;

	LastPrice = ss.LastPrice;
	LastVol = ss.LastVol;
	
	PreSettlement = ss.PreSettlement;
        
	OpenInt = ss.OpenInt;
	TradeVol = ss.TradeVol;
	separator = ss.separator;
}