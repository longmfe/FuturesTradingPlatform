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
	 
	 //��ԼID
    string sec;
    //������λ
    int sellopen;
    //�򿪲�λ
    int buyopen ;
    //������
    double sellAvg ;
    //�����
    double buyAvg ;
    //����ӯ��
    double floatProfit ;
    //��ӯ��
    double BuyProfit  ;
    //��ӯ��
    double SellProfit ;
    //������֤��
    double sellMagin ;
    //�򿪱�֤��
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

