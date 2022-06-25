#include "StdAfx.h"
#include "CTPClass.h"
#include <list>
using namespace std;
//extern GlobalDataManage* pGlobalDm;

CTPClass::CTPClass(AlgoManager* agm1):
	kp("CTPClassLogInfo")
{
	isTradeLogon = false;
	isMDLogon = false;
	this->fixMD = new MarketApplication() ;
	this->fixMD->setFC(this);
	this->fixTrader =new TradeApplication() ;
	this->fixTrader->setFC(this);
	orderRef = 0;
    agm =agm1;
}


CTPClass::~CTPClass(void)
{
	Release();
	map<int, Order*>::iterator it  = orderMap.begin();
	for(;it!=orderMap.end();it++)
	{
		if(it->second!=NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	if(this->fixMD!=NULL)
	{
		
		delete this->fixMD;
		this->fixMD = NULL;
	}
	if(this->fixTrader !=NULL)
	{
		delete this->fixTrader;
		this->fixTrader = NULL;
	}

	map<string, contractDetail*>::iterator it1 = conInfoMap.begin();
	for(;it1 !=conInfoMap.end();it1++)
	{
		if(it1->second!=NULL)
		{
			delete it1->second;
			it1->second = NULL;
		}
	}

}


bool CTPClass::getIsLogonSuccess()
{
	if(isTradeLogon==true&&isMDLogon == true)
    {
        return true;
    }else
    {
        return false;
    }
}
bool CTPClass::getIsTradeLogon()
{
	 return isTradeLogon;
}
void CTPClass::setIsTradeLogon(bool isOK){
	 isTradeLogon = isOK;
}
bool CTPClass::getIsMDLogon(){
	return isMDLogon;
}
void CTPClass::setIsMDLogon(bool isOK){
	 isMDLogon = isOK;

}
void CTPClass::runCTP()
{
	this->login();
} 
    
void CTPClass::requestPosition(){

	 this->fixTrader->requestPosition();

}
void  CTPClass::requestMD(string symbolname ){
	 fixMD->SubcribeMarketData(symbolname);
}

void CTPClass::login()  
{
	this->fixMD->login();
	this->fixTrader->login();
}
//sending newOrderSingle
bool CTPClass::sendorder(Order* order)
{
	 
	order->orderRef = this->orderRef++;
	orderMap.insert(pair<int, Order*>(order->orderRef,order));
	//pGlobalDm->getOrdersMana()->SendedOrder(order); 需要总控模块处理
    if(order->getType()!= Stype::Cancel)
    {
		this->fixTrader->sendNewOrderSingle(order);
    }
    else
    {
       cout<<"error! Pls use the cancelOrder function instead"<<endl;
    } 
    return true;
}

bool CTPClass::cancelOrder(Order* order)
{
	
	if(orderMap.find(order->orderRef)==orderMap.end())
    {
        cout<<"cannot cancel ,no ori order"<<endl;
    }
    else
    {  

		this->fixTrader->cancelOrder(order);
    }     
	return true;
}
    
//void CTPClass::dealExecutionReport()
//{
//
//}
    
    
TradeApplication* CTPClass::getCTPTrader(){
		 
		return fixTrader;

}
 
   
MarketApplication* CTPClass::getCTPMD(){
	 
	return fixMD;
}
 

void CTPClass::UpdateMarket(TickData tdtmp){
	//algotseng->process(tdtmp); no suck fuction in AlgoTSEng 
	// add bypass fucntion
	//void process(string con, double lastp, double lastV, double askp, double askV,
	//	         double bidP, double bidV, double openI, double traddV, string date
	//			 ,string time);

	/*algotseng->process(tdtmp.getSec(),tdtmp.getLastPrice(),tdtmp.getLastVol(),tdtmp.getAskPrice(),
		tdtmp.getAskVol(),tdtmp.getBidPrice(),tdtmp.getBidVol(),
		tdtmp.getOpenInt(),tdtmp.getTradeVol(),tdtmp.getDate(),tdtmp.getTime());*/
	string con = tdtmp.sec;
	double lastp = tdtmp.getLastPrice();
	double lastV = tdtmp.getLastVol();
	double askP = tdtmp.getAskPrice();
	double askV = tdtmp.getAskVol();
	double bidP = tdtmp.getBidPrice();
	double bidV = tdtmp.getBidVol();
	double openI  =tdtmp.getOpenInt();
	double traddV = tdtmp.getTradeVol();
	string date = tdtmp.getDate();
	string time = tdtmp.getTime();
	//algotseng->process(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
	//should add the following fucntion
	// agm->process(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
		//void OnMarketData(string con, double lastp, double lastV, double askp, double askV,
		//         double bidP, double bidV, double openI, double traddV, string date
		//		 ,string time);
	agm->OnMarketData(con,lastp,lastV,askP,askV,bidP,bidV,openI,traddV,date,time);
}
void CTPClass::Release()
{
	if(fixMD!=NULL)
	{
		fixMD->Release();
	}
	if(fixTrader!=NULL)
	{
		fixTrader->Release();
	}
}

void CTPClass::setOrderRef(int of)
{
	if(of>this->orderRef)
	{
		this->orderRef = of;
	}
}


 


void CTPClass::writeLog(string parm)
{
	kp.writeLine(parm);
}

//Aaron 2013-3-6
void CTPClass::QryContractList()
{
   this->fixTrader->sendSecurityListRequest();
}

list<string> CTPClass::FindContracts(string productName)
{
	return this->fixTrader->screenContracts(productName);
}
 



// 用于请求持仓 add 2013.04.18
void CTPClass::requestInstrument(void)
{
	this->fixTrader->sendSecurityListRequest();
}

//add 2013.04.18 用于返回合约名称
// iflag = 0 not the last; iflag =1 the last
void CTPClass::OnRspInstrument(string intrumentName, int iflag)
{
	//for choosing the contracts
	agm->addContracts(intrumentName,iflag);
}
//add 2013.04.18 用于返回持仓，一般是初始化的时候使用
// iflag = 0 not the last; iflag =1 the last
void CTPClass::OnRspPosition(string instrumentname, int dirction, int volumn, double avgprice, int iflag)
{
	//void OnRspPosition(string, int , int, int);//con, direction, lots, islast
	agm->OnRspPosition(instrumentname,dirction,volumn,avgprice,iflag);
}
void  CTPClass::OnRspAccount(double quanyi,double availablemoney)
{
 
   //void OnRspAccount(double, double);
	agm->OnRspAccount(quanyi,availablemoney);

}
void CTPClass::OnOrderVolumnUpdate(ExecReport ertmp)
{
	//是否需要添加 orderID strategyID 等信息
	map<int,Order*>::iterator it = orderMap.find(ertmp.orderRef);
	if(it!=orderMap.end())
	{
		Order* orderptmp  = it->second;
		ertmp.orderRef =  orderptmp->orderID;
		ertmp.StrategyID = orderptmp->StrategyName;
	}
	agm->addPosition(ertmp);
}
void CTPClass::OnOrderStatusUpdate(int orderRef,int orderStatus)
{
	map<int,Order*>::iterator it = orderMap.find(orderRef);	
	if(it!=orderMap.end())
	{
		Order* tmpor = it->second;
		this->agm->OrderUpdate(tmpor->orderID,tmpor->StrategyName,orderStatus);
	}
}
void CTPClass::requestAccount()
{
	this->fixTrader->requestAccount();
}

contractDetail * CTPClass::getContractDetail(string str)
{
	map<string, contractDetail*>::iterator it1=conInfoMap.find(str);

	if(conInfoMap.end()== it1)
	{
		return NULL;
	}else
	{
		return  it1->second;
	
	}
}
void CTPClass::addContractDetail(string con, contractDetail * cond)
{
	conInfoMap.insert(pair<string, contractDetail*>(con,cond));
}
void CTPClass::updateContractDetailByMD(string con, double UpperLimitPrice, double LowerLimitPrice, double lastPrice )
{

	map<string, contractDetail*>::iterator it1=conInfoMap.find(con);
	it1->second->lastPrice = lastPrice;
	it1->second->UpperLimitPrice = UpperLimitPrice;
	it1->second->LowerLimitPrice = LowerLimitPrice;

}