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
	// ���ս��
	double YEquity;
	// ����Ȩ��
	double Equity;
	//����ӯ��
    double FloatProfits;
    //ƽ��ӯ��
    double ClosePositonProfits;
    //��֤��
    double Margin ;
    //���ᱣ֤��
    double Fronemargin ;
    //�����ʽ�
    double FreeMoney;
    //����������
    double FroneComm ;
    //������
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


