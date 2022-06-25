#pragma once
#include <map>
#include <string>
#include "Order.h"
#include "Position.h"
#include "Security.h"
#include "toolsClass.h"
#include "GlobalDataManage.h"
#include "TickData.h"
#include "ExecReport.h" 

using namespace std;
class Position;
class GlobalDataManage;
class Order;
class ExecReport;
class PositionManage
{
public:
	PositionManage(GlobalDataManage*);
	~PositionManage(void);

	map<string, Position*> orderMap;
	string split ;
	void addOrder(Order* or);
	int getNumOfOrder();
	
	Position* getPositionBysec(string sec);
	string getString();
	
	void computeFloatProfit(TickData sec);

    bool cleanForNewDay();
	void addExecReport(ExecReport er);
private:
	GlobalDataManage* m_am;
};

