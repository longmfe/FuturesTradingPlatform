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


void MarketApplication::onMarketdata(TickData td)
{
	this->fc->UpdateMarket(td);
}
