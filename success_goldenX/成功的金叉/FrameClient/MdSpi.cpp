#include "stdafx.h"
#include "MdSpi.h"
#include <iostream>
using namespace std;
#pragma warning(disable : 4996)

UINT SendMarketRequestThreadFunc(LPVOID lp)
{	
	CMdSpi * m = (CMdSpi*)lp;
	if(m!=NULL)
	{
		m->autoRequestMarket();
	}
	return 0;
}




// USER_API参数
// 配置参数
// 请求编号
CMdSpi::CMdSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcMdApi *ppUserapi,MarketApplication *cctphandle)
{	
	strncpy(BROKER_ID,brokerID,sizeof(BROKER_ID));
	pUserApi=ppUserapi;
	iRequestID = 0;
	this->ctp = cctphandle;
	string tmp =brokerID;
	strcpy(this->BROKER_ID,tmp.c_str());
	preVolum= 0;
	prepreVolum = 0;
}
void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}
/*------------------------------------------------------------------------------
Name:
	OnFrontDisconnected
Parm:
	int nReason
Return:

Description
	the call back function for the disconnecting of the connection to the CTP 
------------------------------------------------------------------------------*/
void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
	//this->ctp->setTradeLogonFlag(false);
	//this->Login();
	this->prepreVolum =0;
	this->preVolum =0;
	this->ctp->setLogonStatus(false);
}		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

/*------------------------------------------------------------------------------
Name:
	OnFrontConnected
Parm:

Return:

Description
	the call back function for successfully connecting to the CTP server 
------------------------------------------------------------------------------*/
void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << "OnFrontConnected" << endl;
	this->Login();
}
/*------------------------------------------------------------------------------
Name:
	ReqUserLogin
Parm:
	TThostFtdcInvestorIDType account
	TThostFtdcPasswordType password
Return:

Description
	To send the login request to the CTP server
------------------------------------------------------------------------------*/
void CMdSpi::ReqUserLogin(TThostFtdcInvestorIDType account,TThostFtdcPasswordType password)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, account);
	strcpy(req.Password, password);
	strcpy(INVESTOR_ID,account);
	strcpy(PASSWORD,password);
	 
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	
}
/*------------------------------------------------------------------------------
Name:
	OnRspUserLogin
Parm:
	CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function of the login request
------------------------------------------------------------------------------*/
void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << "OnRspUserLogin" << endl;
	cout<< "MdLogin!!!"<<endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay() << endl;
		this->ctp->setLogonStatus(true);
		//this->marketSymbolMap.insert(pair<string,string>("CF305","CF305"));
		//this->marketSymbolMap.insert(pair<string,string>("cu1302","cu1302"));
		AfxBeginThread(SendMarketRequestThreadFunc,this);
	 
	}else
	{
		
	}
}
/*------------------------------------------------------------------------------
Name:
	SubsribeMarketData
Parm:
	
Return:

Description
	To book the market data for the user
------------------------------------------------------------------------------*/
void CMdSpi::SubscribeMarketData()
{
	char ppInstrumentID[1000][31] = {0};
	char *pptmp[1000]={0};
	/*list<CString> tmplist = APIctrl->getContractList();
	list<CString>::iterator it = tmplist.begin();
	int x = tmplist.size();
	this->mdlg->WriteLog("开始发送行情请求");
	for (int i =0;i<x;i++)
	{
		strncpy(ppInstrumentID[i],(*it).GetBuffer(),31);
		pptmp[i]=ppInstrumentID[i];
		(*it).ReleaseBuffer();
		it++;
	}
	int iResult = pUserApi->SubscribeMarketData(pptmp,x);
	this->mdlg->WriteLog("行情请求发送完成");*/
	//cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}
void CMdSpi::SubscribeMarketData(string symbol)
{
	if(marketSymbolMap.find(symbol)==marketSymbolMap.end())
	marketSymbolMap.insert(pair<string,string>(symbol,symbol));
	char ppInstrumentID[1][31] = {0};
	strncpy(ppInstrumentID[0],symbol.c_str(),31);
	char *pptmp[1]={0};
	pptmp[0] =  ppInstrumentID[0];
	int iResult = pUserApi->SubscribeMarketData(pptmp,1);
}
void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "OnRspSubMarketData" << endl;
}
void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "OnRspUnSubMarketData" << endl;
}
/*------------------------------------------------------------------------------
Name:
	OnRtnDepthMarketData
Parm:
	CThostFtdcDepthMarketDataField *pDepthMarketData
Return:

Description
	The call back function of the market data request
------------------------------------------------------------------------------*/
void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cerr << "OnRtnDepthMarketData" << endl;
	//cerr <<pDepthMarketData <<endl;
	if(pDepthMarketData!=NULL)
	{
		TickData ct;
		ct.sec =pDepthMarketData->InstrumentID;
		ct.setAskPrice(pDepthMarketData->AskPrice1);
		ct.setAskVol(pDepthMarketData->AskVolume1);
		ct.setBidPrice(pDepthMarketData->BidPrice1);
		ct.setBidVol(pDepthMarketData->BidVolume1);
		ct.setLastPrice(pDepthMarketData->LastPrice);
		//if(pDepthMarketData->Volume ==0
		//map<string,int> marketPrVolum;
		//this->ctp->updateContractDetail(pDepthMarketData->InstrumentID,pDepthMarketData->UpperLimitPrice,pDepthMarketData->LowerLimitPrice,pDepthMarketData->LastPrice);
		map<string,int>::iterator it= marketPrePreVolum.find(ct.sec);
		if(it==marketPrePreVolum.end()) {
			marketPrePreVolum.insert(pair<string,int>(ct.sec,0));
			marketPreVolum.insert(pair<string,int>(ct.sec,0));
			ct.setLastVol(0); 
		}else
		{
		   if(pDepthMarketData->Volume == 1.7976931348623157e+308)
		   {
		      ct.setLastVol(0); 
		   }else
		   {

			  map<string,int>::iterator PrePreit= marketPrePreVolum.find(ct.sec);
			  map<string,int>::iterator Preit= marketPreVolum.find(ct.sec);

		      if(Preit==marketPreVolum.end())
			  {
                   	marketPreVolum.insert(pair<string,int>(ct.sec,0));   
					ct.setLastVol(0); 
			  }else
			  {
				   int PrePreint = PrePreit->second;
				   int Preint = Preit->second;
				   if(PrePreint==0&&Preint==0)
				   {

				       ct.setLastVol(pDepthMarketData->Volume);
					   Preit->second = pDepthMarketData->Volume;
				   }else if(Preint!=0&&PrePreint==0)
				   {
				       PrePreit->second = Preit->second ;
				       ct.setLastVol(pDepthMarketData->Volume-Preint);
					   Preit->second = pDepthMarketData->Volume;
				   }else
				   {
				       PrePreit->second = Preit->second ;
				       ct.setLastVol(pDepthMarketData->Volume-Preint);
					   Preit->second = pDepthMarketData->Volume;
				   }  
			  }
		   }
	    }

		//if(pDepthMarketData->Volume == 1.7976931348623157e+308)
		//{
		//	ct.setLastVol(0);
		//}else
		//{   
		//	if(this->preVolum == 0&&prepreVolum==0)
		//	{
		//		this->preVolum = pDepthMarketData->Volume;
		//		ct.setLastVol(0);
		//	}else if(this->preVolum!=0&&prepreVolum==0)
		//	{
		//		this->prepreVolum = this->preVolum;
		//		ct.setLastVol(0);
		//		this->preVolum = pDepthMarketData->Volume;	 
		//	}else 
		//	{
		//		this->prepreVolum = this->preVolum;
		//		ct.setLastVol(pDepthMarketData->Volume-this->preVolum);
		//		this->preVolum = pDepthMarketData->Volume;	 
		//	}
		//
		//}
		ct.setOpenInt(pDepthMarketData->OpenInterest);
		string tmpTime = pDepthMarketData->UpdateTime;
		toolsClass tc;
		tmpTime = tc.deleteSubStr(tmpTime,':');
		if(tmpTime.size()<6)
		{
			tmpTime = "0"+tmpTime;
		}
		int tim11 = pDepthMarketData->UpdateMillisec;
		if(tim11!=500)
		{
			tmpTime=tmpTime+".000";
		}else
		{
			tmpTime=tmpTime+".500";
		}

	 
		ct.setTime(tmpTime);
		//ct.sec =pDepthMarketData->InstrumentID;
		ct.setTradeVol(pDepthMarketData->Volume);
		ct.setDate(pDepthMarketData->TradingDay);
		//ct.setUpLimitPrice(pDepthMarketData->UpperLimitPrice);
		//ct.setLowLimitPrice(pDepthMarketData->LowerLimitPrice);
		this->ctp->onMarketdata(ct);
		cout<<"get data"<<pDepthMarketData->InstrumentID<<","<<pDepthMarketData->AskPrice1<<endl;
	
	}
}
bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}


void CMdSpi::setUserName(string name)
{
	strcpy(this->INVESTOR_ID,name.c_str());
}
void CMdSpi::setPassword(string password)
{
	strcpy(this->PASSWORD,password.c_str());
}


void CMdSpi::Login(void)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, this->BROKER_ID);
	strcpy(req.UserID,this->INVESTOR_ID);
	strcpy(req.Password,this->PASSWORD);
	 
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
}
void CMdSpi::autoRequestMarket()
{
		map<string,string>::iterator it = marketSymbolMap.begin();
		if(marketSymbolMap.size()==0)
		{
		
		}else
		{
			for(;it!=marketSymbolMap.end();it++)
			{
				this->SubscribeMarketData(it->first);
			}
		}
	
}