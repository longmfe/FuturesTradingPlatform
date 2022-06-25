#pragma once
#include <string>
#include "Lock.h"
#include <sstream>
using namespace std;
class AccountManage
{
public:
	AccountManage();
	 
	AccountManage(string name, double ye);
	~AccountManage(void);

	string Name;
    string Password;
	// 昨日结存
	double YEquity;
	// 今日权益
	double Equity;
	//浮动盈亏
    double FloatProfits;
    //平仓盈亏
    double ClosePositonProfits;
    //保证金
    double Margin ;
    //冻结保证金
    double Fronemargin ;
    //可用资金
    double FreeMoney;
    //冻结手续费
    double FroneComm ;
    //手续费
    double Commision ;
    string split;
    //static lock
private:
	CLock lock;
public:
	string getName();
	void setPassword(string value);
	double getYEquity();
	void setYEquity(double value);
	double getEquity();
	double getFloatProfits();
	void setFloatProfits(double value);
	double getClosePositonProfits();
	void setClosePositonProfits(double value);
	void addClosePositonProfits(double value);
	double getMargin();
	void setMargin(double value);
	void addMargin(double value);
	double getFronemargin();
	void setFronemargin(double value);
	bool addFroneMargin(double value);
	double getFreeMoney();
	double getCommision();
	void setCommision(double value);
	void addCommision(double value);
	double getFroneComm();
	void setFroneComm(double value);
	bool addFroneCommision(double value);
	string toString();
	string doubleToString(double value);
};


