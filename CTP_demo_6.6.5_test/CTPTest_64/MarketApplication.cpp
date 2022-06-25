#include "StdAfx.h"
#include "MarketApplication.h"
//#include "readProperty.h"
//#include "UtilFun.h"
MarketApplication* ta= nullptr;



MarketApplication::MarketApplication(void)//:
//kc("MarketSessionLogInfo")
{
	isLogon =false;
	rm=NULL;
	pUserMdSpi = NULL;
	ta = this;
	
	
}

MarketApplication::MarketApplication(CTPClass* ctp)//:
//kc("MarketSessionLogInfo")
{
	isLogon = false;
	rm = NULL;
	pUserMdSpi = NULL;
	ta = this;
	this->fc = ctp;

}

MarketApplication::~MarketApplication(void)
{
	if(rm!=NULL)
	{
		delete rm;
		rm =NULL;
	}
	if(pUserMdSpi!=NULL)
	{
		delete pUserMdSpi;
		pUserMdSpi =NULL;
	}
}

void MarketApplication::setFC(CTPClass* fc)
{
	this->fc = fc;

}

void MarketApplication::onMarketdata(TickData td)
{
	cout << "MarketApplication::onMarketdata" << endl;
	
	this->fc->UpdateMarket(td);
}
