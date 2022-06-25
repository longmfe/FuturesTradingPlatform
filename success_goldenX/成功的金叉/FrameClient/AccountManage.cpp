#include "StdAfx.h"
#include "AccountManage.h"

AccountManage::AccountManage()
	: YEquity(0)
	, Equity(0)
	,FloatProfits(0)
	,ClosePositonProfits(0)
	,Margin(0)
	,Fronemargin(0)
	,FreeMoney(0)
	,FroneComm(0)
	,Commision(0)
{
		split= ",";
		Name = "N/A";
		Password = "N/A"; 
}
AccountManage::AccountManage(string name, double ye)
	: YEquity(0)
	, Equity(0)
	,FloatProfits(0)
	,ClosePositonProfits(0)
	,Margin(0)
	,Fronemargin(0)
	,FreeMoney(0)
	,FroneComm(0)
	,Commision(0)
{
		split= ",";
		Name = "N/A";
		Password = "N/A";
		Name = name;
        YEquity = ye;
        FreeMoney = ye;
}

AccountManage::~AccountManage(void)
{
}

string AccountManage::getName()
{
	return this->Name;
}

void AccountManage::setPassword(string value)
{
	this->Password = value;
}
double AccountManage::getYEquity()
{
	return this->YEquity;
}
void AccountManage::setYEquity(double value)
{
	this->YEquity =value;
}
double AccountManage::getEquity()
{
	double tt = FreeMoney + Margin + Fronemargin + FroneComm + FloatProfits;
    return tt;
}
double AccountManage::getFloatProfits()
{
	return this->FloatProfits;
}
void AccountManage::setFloatProfits(double value)
{
	this->FloatProfits = value; 
}

double AccountManage::getClosePositonProfits()
{
	return ClosePositonProfits;
}
void AccountManage::setClosePositonProfits(double value)
{
	 FreeMoney += value - ClosePositonProfits;   
     ClosePositonProfits = value;
}

void AccountManage::addClosePositonProfits(double value)
{
        lock.lock();
        ClosePositonProfits += value;
        FreeMoney += value;   
        lock.unlock();
}

double AccountManage::getMargin()
{
        if(Margin < 0.0000001){
            return 0;
        }
        return Margin;
 }
void AccountManage::setMargin(double value)
{
        FreeMoney += Margin - value;   
        //FreeMoney = FreeMoney + Margin - value;
        Margin = value;            
}

 void AccountManage::addMargin(double value)
{
        lock.lock();
        Margin += value;
        FreeMoney -= value;   
        lock.unlock();
}
double AccountManage::getFronemargin()
{
        if(Fronemargin < 0.00001){
            return 0;
        }
        return Fronemargin;
}
 void AccountManage::setFronemargin(double value)
 {
        FreeMoney += Fronemargin - value;   
            //FreeMoney = FreeMoney + Fronemargin - value;
        Fronemargin = value;
 }
 bool  AccountManage::addFroneMargin(double value)
 {
        bool  res = true;
        lock.lock();
        if(FreeMoney - value < 0){
            lock.unlock();
            return false;
        }
        Fronemargin += value;
        FreeMoney -= value;   
        lock.unlock();
        return res;
}

double  AccountManage::getFreeMoney(){
        return FreeMoney;
}
double  AccountManage::getCommision()
{
            return Commision;
}
void  AccountManage::setCommision(double value)
{
        FreeMoney += Commision - value;   
        //FreeMoney = FreeMoney + Commision - value;
        Commision = value;
}
void  AccountManage::addCommision(double value)
{
        lock.lock();
        Commision += value;
        FreeMoney -= value;   
        lock.unlock();
}
double AccountManage::getFroneComm()
{
        if(FroneComm < 0.0000001){
            return 0;
        }

        return FroneComm;
}
void AccountManage::setFroneComm(double value){
        FreeMoney += FroneComm - value;
        //FreeMoney = FreeMoney + FroneComm - value;
        FroneComm = value;
}
bool AccountManage::addFroneCommision(double value)
{
        bool res = true;
        lock.lock();
        if(FreeMoney - value < 0){
            lock.unlock();
            return false;
        }
        FroneComm += value;
        FreeMoney -= value;   
        lock.unlock();
        return res;
}
string AccountManage::toString()
{
	string str = "";
	str = "ID:" + Name + split + "YEquity:" + doubleToString(YEquity) + split + "Equity:" + doubleToString(getEquity()) + split
                    + "FloatProfits:" + doubleToString(FloatProfits) + split + "ClosePositonProfits:" + doubleToString(ClosePositonProfits) + split
                    + "Magin:" + doubleToString(Margin) + split + "Fronemargin:" + doubleToString(Fronemargin) + split
                    + "FreeMoney:" + doubleToString(FreeMoney) + split + "FroneComm:" + doubleToString(FroneComm) + split
                    + "Commision:" + doubleToString(Commision);		
    return str;
}
string AccountManage::doubleToString(double value)
{
	stringstream ss; 
	string result; 
	ss << value; 
	ss >>result;  
	return result;
}
