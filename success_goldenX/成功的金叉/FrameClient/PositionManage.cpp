#include "StdAfx.h"
#include "PositionManage.h"


PositionManage::PositionManage(GlobalDataManage* am)
{
	split="---";
	m_am = am;
}


PositionManage::~PositionManage(void)
{

}

void PositionManage::addOrder(Order* or){
	map<string, Position*>::iterator it = orderMap.find(or->getSec());
	if(it!= orderMap.end())
	{
		it->second->addPosition(*or);
	}else
	{
		Position * po  = new  Position(*or, m_am);
		orderMap.insert(pair<string,Position*>(po->getSec(), po));
	}
}
int PositionManage::getNumOfOrder()
{
		return orderMap.size();
}
Position* PositionManage::getPositionBysec(string sec)
{
	toolsClass tc ;
	//string tmpSecstr = tc.getPreStrBySplit(sec," ");
	map<string, Position*>::iterator it = orderMap.find(sec);
	if(it!= orderMap.end())
	{
		return it->second;
	}else
	{
		Position* p  = new Position(sec, m_am);
		orderMap.insert(pair<string,Position*>(sec,p));
		return p;
	}
}
string PositionManage::getString()
{
		string res = "";
		toolsClass tc;
		 
		map<string, Position*>::iterator it = orderMap.begin();
		while(it!=orderMap.end())
		{
			Position* po = it->second;
			res += "Sec:" + po->getSec() + split + "FloatProfit:" + tc.doubleToString(po->getFloatProfit()) + split
			      + "BuyProfit:" +tc.doubleToString(po->getBuyProfit()) + split + "ButOpen:" + tc.doubleToString(po->getBuyopen())
				  + split+ "BuyAvgPrice:" + tc.doubleToString(po->getBuyAvg())+ split + "SellProfit:" 
				  + tc.doubleToString(po->getSellProfit()) + split
			      + "SellOpen:" + tc.doubleToString(po->getSellopen())
				  + split + "SellAvgPrice:" + tc.doubleToString(po->getSellAvg()) + "\r\n";
			it++;
		}
		return res;
}
bool PositionManage::cleanForNewDay()
{
	bool res = true;
            
	map<string, Position*>::iterator it = orderMap.begin();
	for(;it !=orderMap.end();it++)
	{
		delete it->second;
	}
	orderMap.clear();        
    return res;
}
void PositionManage::addExecReport(ExecReport er)
{
	if(orderMap.find(er.getSec())!=orderMap.end()){
		orderMap.find(er.getSec())->second->addExecReport(er);
		//AlgoTSEng.GlobalDm.keepRecord(FileType.TradeDetail, or.getTradeString());
		return;
	}		
	Position* po = new Position(er.getSec(), m_am);
	po->addExecReport(er);
	orderMap.insert(pair<string,Position*>(po->getSec(), po));
}