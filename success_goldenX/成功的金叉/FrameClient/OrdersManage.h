#pragma once
#include <string>
#include <list>
#include "Order.h"
#include "GlobalDataManage.h"
#include "toolsClass.h"
using namespace std;
class Order;
class GlobalDataManage;
class OrdersManage
{
public:
	OrdersManage(GlobalDataManage*);
	~OrdersManage(void);
	list<Order* > HisOrder;
	list<Order* > unTradeOrder;
	string split ;
	int orderID ;
	
 
	void SendedOrder(Order* or);
	
	int  getLastOrderID();
	
	
	list<Order*> getHisOrder();
	
	list<Order*> getUnTradeOrder();
        
    Order* getOrderByID(int id);
        
        //get
    int getSumOfUnTradeOrder();
    int getSumOfUnTradeOrder(string con);
        
        //cancel order
    bool CancelOrder(int id);
	bool CancelAll();
	bool CancelAll(string con);
private:
	void removeOrder(int id);
	void removeOrder(Order* or);
	GlobalDataManage* m_pGlobalDm;


        
public:
	void ModifySendingOrder(Order* or);
	
	string getString();
	list<int> getUnTradeOrderID(string con);
	void RemoveUnTradeOrder(Order* or);
	// remove order from untraderorder list when the order is filled or canceled or rejected, you can still find it in the hisorderList
	bool removeOrderFromUnTraderOrderByID(int orderid);

};

