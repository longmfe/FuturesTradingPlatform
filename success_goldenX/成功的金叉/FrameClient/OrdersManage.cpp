#include "StdAfx.h"
#include "OrdersManage.h"
#include "GlobalDataManage.h"
#include "toolsClass.h"
toolsClass tc;

extern AlgoManager* algomanager;

OrdersManage::OrdersManage(GlobalDataManage* pGlobalDm):
 split("---"),
 orderID(1000)
{
	m_pGlobalDm = pGlobalDm;
}

OrdersManage::~OrdersManage(void)
{
	 list<Order*>::iterator it = unTradeOrder.begin();
	 for(;it!=unTradeOrder.end();it++)
	 {
		Order* tmp = *it;
		if(tmp!=NULL)
		{
			delete tmp;
		}
		tmp = NULL;
	 }
	 list<Order*>::iterator it1 = HisOrder.begin();
	 for(;it1!=HisOrder.end();it1++)
	 {
		Order* tmp = *it1;
		if(tmp!=NULL)
		{
			delete tmp;
		}
		tmp = NULL;
	 }
}

void OrdersManage::SendedOrder(Order* or)
{
		HisOrder.push_back(or);
		unTradeOrder.push_back(or);
}

int   OrdersManage::getLastOrderID()
{
		orderID++;
		return orderID;
}

list<Order*> OrdersManage:: getHisOrder()
{
		return HisOrder;
}

list<Order*>  OrdersManage::getUnTradeOrder()
{
		return unTradeOrder;
}

Order* OrdersManage::getOrderByID(int id)
{
	 list<Order*>::iterator it = unTradeOrder.begin();
	 for(;it!=unTradeOrder.end();it++)
	 {
		 if((*it)->getOrderID()==id)
		 {
			return *it;
		 }
	 
	 }
	 
	 return NULL;      
}

list<int> OrdersManage::getUnTradeOrderID(string con)
{
     list<int> tmpList ;
	 list<Order*>::iterator it = unTradeOrder.begin();   
	 for(;it!=unTradeOrder.end();it++)
	 {
		 if((*it)->sec.compare(con) == 0)
		 {
		 	tmpList.push_back((*it)->getOrderID());
		 }
	 }
     return tmpList;
}

int OrdersManage::getSumOfUnTradeOrder()
{
            
      return unTradeOrder.size();
}

int OrdersManage::getSumOfUnTradeOrder(string con)
{
	if(getUnTradeOrderID(con).size()==0)
	{
                return 0;
    }
    return getUnTradeOrderID(con).size();
}

bool  OrdersManage::CancelOrder(int id)
{
    bool res = false;
      
	try
	{
		list<Order*>::iterator it = unTradeOrder.begin();
		//int i =0;
		for(;it!=unTradeOrder.end();it++)
		{
			if((*it)->getOrderID()== id)
			{ 
				AccountManage* am = algomanager->getAccountMana();
				double tmp2 = am->getFroneComm();
				double tmp3 = am->getFronemargin();

				double tmp = 0;
				tmp = tmp2 - (*it)->getFroneCommision();
				am->setFroneComm(tmp);

				tmp = tmp3 - (*it)->getFroneMargin();
				am->setFronemargin(tmp);
				res = true;
				m_pGlobalDm->keepRecord(CancelOrders, "CancelOrder,"+tc.IntToString((*it)->getOrderID()));
				it = unTradeOrder.erase(it);

				break;
			}
			//else
			//{
			//	it++;
			//}
			//i++;
		}

		if(res)
		{
			cout<<"Cancel by id---------------"<<id<<"succeed!"<<endl;
		}
		else
		{
			cout<<"Cancel by id---------------"<<id<<"fail!"<<endl;
		}
	}
	catch(CException* e)
	{
		cout<<"Cancel by id---------------"<<"CException!"<<endl;
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024);   //  e.GetErrorMessage(szError,1024);
		m_pGlobalDm->keepRecord(Account,"CancelByID exception");
	}
	catch(...)
	{

	}

    return res;
}

bool OrdersManage::CancelAll()
{
	cout<< "cancel all"<<unTradeOrder.size()<<endl;
	bool res = false;        
	try
	{
		list<Order*>::iterator it = unTradeOrder.begin();
		int i = 0;

		for(;it!=unTradeOrder.end();it++)
		{
			if(*it==NULL)
			{
				cout<< "CancelAll ERROR"<<endl;
			}
			AccountManage* am = algomanager->getAccountMana();
			double tmp2 = am->getFroneComm();
			double tmp3 = am->getFronemargin();

			double tmp = 0;

			tmp = tmp2 - (*it)->getFroneCommision();
			am->setFroneComm(tmp);
			tmp = tmp3 - (*it)->getFroneMargin();
			am->setFronemargin(tmp);
			m_pGlobalDm->keepRecord(CancelOrders, "CancelAll(),"+tc.IntToString((*it)->getOrderID()));
			//it = unTradeOrder.erase(it);
			i++;
		}     
			res = true;
		unTradeOrder.clear();
		cout<<"Cancel All,totally: "<<i<<endl;
	}
	catch(CException* e)
	{
		cout<<"CancelAll---------------"<<"CException!"<<endl;
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024); 
		m_pGlobalDm->keepRecord(Account,"CancelAll exception");
	}
	catch(...)
	{

	}
    return res;
}

bool OrdersManage::CancelAll(string con)
{
	bool res = false; 
	try
	{
		list<Order*>::iterator it = unTradeOrder.begin();
		int i = 0;
		for(;it!=unTradeOrder.end();)
		{
			if((*it)->sec.compare(con) ==0)
			{
				AccountManage* am = algomanager->getAccountMana();
				double tmp2 = am->getFroneComm();
				double tmp3 = am->getFronemargin();
				double tmp = 0;
				tmp = tmp2 - (*it)->getFroneCommision();
				am->setFroneComm(tmp);
				tmp = tmp3 - (*it)->getFroneMargin();
				am->setFronemargin(tmp);
				m_pGlobalDm->keepRecord(CancelOrders, "CancelAll(con),"+tc.IntToString((*it)->getOrderID()));
				it = unTradeOrder.erase(it);
				res = true;
				i++;
			}
			else
			{
				it++;
			}
		}            
		cout<<"Cancel All with constring,totally: "<<i<<endl;
	}
	catch(CException* e)
	{
		cout<<"CancelAllByCon---------------"<<"CException!"<<endl;
		TCHAR   szError[1024];   
		e->GetErrorMessage(szError,1024); 
		m_pGlobalDm->keepRecord(Account,"CancelAllByCon exception");
	}
	catch(...)
	{

	}
    return res;
}
  
void OrdersManage::removeOrder(int id)
{
    AccountManage* am = algomanager->getAccountMana();
    double tmp2 = am->getFroneComm();
    double tmp3 = am->getFronemargin();

    double tmp = 0;

	list<Order*>::iterator it = unTradeOrder.begin();
	int i =0;
	while(i<id)
	{
		it++;
		i++;
	}
    tmp = tmp2 - (*it)->getFroneCommision();
    am->setFroneComm(tmp);

    tmp = tmp3 - (*it)->getFroneMargin();
    am->setFronemargin(tmp);
	Order* tmpor = *it;
	it++;
	unTradeOrder.remove((tmpor));
	delete tmpor;
            
}

void OrdersManage::removeOrder(Order* or)
{
	AccountManage* am = algomanager->getAccountMana();
	double tmp2 = am->getFroneComm();
	double tmp3 = am->getFronemargin();

	double tmp = 0;

	tmp = tmp2 - (or)->getFroneCommision();
	am->setFroneComm(tmp);

	tmp = tmp3 - (or)->getFroneMargin();
	am->setFronemargin(tmp);
	//Order* tmpor = or;
	unTradeOrder.remove(or);
	//delete tmpor;

}

 void  OrdersManage::ModifySendingOrder(Order* or)
{
		//
}

string OrdersManage::getString(){
	string str = "";
	string di = "";
	toolsClass tc;
	list<Order*>::iterator it = unTradeOrder.begin();
	for( ;  it!= unTradeOrder.end(); it ++){
		Order* t = (*it);			
		if(t->getType() == Stype::Buy){
			di = "buyopen";
		}
		else if(t->getType() == Stype::Buytocover){
			di = "buytocover";
		}
		else if(t->getType() == Stype::Sell){
			di = "sellcover";
		}
		else if(t->getType() == Stype::Sellshort){
			di = "sellshort";
		}
		str += "Id:" + orderID + split + "Sec:" + t->getSec() + split + "Direction:" + di + split + "Price:" 
			+ tc.doubleToString(t->getSendPrice()) + split
			        + "Trading lots" + tc.doubleToString(t->getTradeVolume())
					+ split + "Total Vol:" + tc.doubleToString(t->getSendVol())
					+ split + "Status:" + tc.doubleToString(t->getStatus()) + "\r\n";
	}
	return str;
}

//for dealOrderLocal 
void OrdersManage::RemoveUnTradeOrder(Order* or)
{
	unTradeOrder.remove(or);
}

// remove order from untraderorder list when the order is filled or canceled or rejected, you can still find it in the hisorderList
// During ctp real test ,you have to update the  UnTraderOrder.
bool OrdersManage::removeOrderFromUnTraderOrderByID(int orderid)
{
	bool res = false;         
	list<Order*>::iterator it = unTradeOrder.begin();
	int i =0;
	for(;it!=unTradeOrder.end();it++)
	{
		if((*it)->getOrderID()==orderid)
		{
			 removeOrder(*it);
			 return true;
		}
		i++;
	}
    return res; 
}
