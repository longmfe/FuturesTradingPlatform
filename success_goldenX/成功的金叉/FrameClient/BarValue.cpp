#include "StdAfx.h"
#include"BarValue.h"
#include "toolsClass.h"
BarValue::BarValue(void)
{

}
BarValue::~BarValue(void)
{

}
BarValue::BarValue(string date,string time,double open,double close,
	double high,double low,double tradevolume,double openinterest,string realContractID,string nameContractID)
{
	this->date = date;
	this->time = time;
	this->open = open;
	this->close = close;
	this->high = high;
	this->low = low;
	this->tradevolume = tradevolume;
	this->openinterest = openinterest;
	this->realContractID = realContractID;
	this->nameContractID = nameContractID;
	this->separator = "_";
	
	this->macdDiff=0.0;
}
string BarValue::Object2String()
{
	toolsClass tc;
   return 
	   this->realContractID + this->separator +  
	   this->date + this->separator +  
	   this->time + this->separator + 
	   tc.doubleToString(this->open) + this->separator +  
	   tc.doubleToString(this->high) + this->separator +  
	   tc.doubleToString(this->low) + this->separator +  
	   tc.doubleToString(this->close) + this->separator +  
	   tc.doubleToString(this->tradevolume);
}

void BarValue::setDiff(double diff){
	macdDiff=diff;
}

double BarValue::getDiff(){
	return macdDiff;
}