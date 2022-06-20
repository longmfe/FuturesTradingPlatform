
#pragma once
#include "stdafx.h"
#include <windows.h>
#include <iostream>
using namespace std;

#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "TraderSpi.h"
 
#pragma warning(disable : 4996)
 
// ������
// int iRequestID;
 

// �Ự����
/*------------------------------------------------------------------------------
Name:
	SendMoneyMessageThreadFunc
Parm:
	LPVOID lp
Return:
	UINT
Description
	To send the user's money message 
------------------------------------------------------------------------------*/
UINT SendMoneyMessageThreadFunc(LPVOID lp)
{	
	CTraderSpi * m = (CTraderSpi*)lp;
	if(m!=NULL)
	{
		//m->getAPICtrl()->sendMoneyMessage();
		m->replyInstrumentRequest();
	}
	return 0;
}
/*------------------------------------------------------------------------------
Name:
	SendMoneyMessageThreadFunc
Parm:
	LPVOID lp
Return:
	UINT
Description
	To send the user's money message 
------------------------------------------------------------------------------*/
UINT SendInstrumentReplyhreadFunc(LPVOID lp)
{	
	CTraderSpi * m = (CTraderSpi*)lp;
	if(m!=NULL)
	{
		//m->getAPICtrl()->sendMoneyMessage();
	    // replyInstrumentRequest
		m->replyInstrumentRequest();
	}
	return 0;
}
// �����ж�
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

CTraderSpi::CTraderSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcTraderApi *ppUserapi)
{
	this->ISMAPFILLED=false;
	this->ISMAPFILLEDFINISHED=false;
//	this->cth1 =cth;
	string tmp = brokerID;
	strcpy(this->BROKER_ID,tmp.c_str());
	this->pUserApi = ppUserapi;
	iRequestID = 0;
	currentDate= "";
}
void CTraderSpi::OnFrontConnected()
{
	cerr << "--->>> " << "traderOnFrontConnected" << endl;
	///�û���¼����
	///ReqUserLogin();
	Login();
	 
}
/*------------------------------------------------------------------------------
Name:
	OnRspUserLogin
Parm:
	CThostFtdcRspUserLoginField *pRspUserLogin
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function of the Login request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout<< "traderLogin!!!"<<endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		sessionInFoMap.insert(pair<TThostFtdcSessionIDType,TThostFtdcFrontIDType>(SESSION_ID,FRONT_ID));
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
//		this->cth1->setOrderRef(iNextOrderRef);
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///��ȡ��ǰ������
	
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		if(currentDate.size()==0)
		{
			currentDate =  pUserApi->GetTradingDay();
		}else
		{
			string tmpdate =  pUserApi->GetTradingDay();
			if(strcmp(currentDate.c_str(),tmpdate.c_str())==0)
			{
				
			}else
			{
				currentDate = tmpdate;
				this->clearTodayPosition();
			}
		}
		///Ͷ���߽�����ȷ��
	    ReqSettlementInfoConfirm();
		cout<<"loginRspEnd"<<endl;
	}
}
/*------------------------------------------------------------------------------
Name:
	ReqSettlementInfoConfirm
Parm:
	
Return:

Description
	The function to confirm the user's settlement,it should be called before user do
	the trading
------------------------------------------------------------------------------*/
void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	
	//cerr << "--->>> Ͷ���߽�����ȷ��: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
}
/*------------------------------------------------------------------------------
Name:
	OnRspSettlementInfoConfirm
Parm:
	CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	the call back function of the Settlement confirm request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		ReqQryInstrument();
	}
}
 
 
/*------------------------------------------------------------------------------
Name:
	ReqQryInvestorPosition
Parm:

Return:

Description
	The function that requests the user's position from the CTP
------------------------------------------------------------------------------*/
void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			 
			break;
		}
		else
		{
			//cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ", �ܵ�����" << endl;
			//if(!APIctrl->isThisAvalable)
			//{
			//	return;
			//}
			//mdlg->WriteLog("ReqQryInvestorPosition ����ѭ��");
			Sleep(1000);
		}
	}
	 
}
/*------------------------------------------------------------------------------
Name:
	OnRspQryInvestorPosition
Parm:
    CThostFtdcInvestorPositionField *pInvestorPosition
    CThostFtdcRspInfoField *pRspInfo
    int nRequestID
    bool bIsLast
Return:

Description
	The call back function of the user's position request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	// cp1;
	//todayPositionMap;
	if(pInvestorPosition==NULL)
	{
		return;
	}
//	ExecReport er;
	string symbol = pInvestorPosition->InstrumentID;
	int todayVolum=0;
	double avgPrice =0;
	Stype direction ;
	int totalVolum = 0;
	if(THOST_FTDC_PD_Long == pInvestorPosition->PosiDirection)
	{
		direction = Stype::Buy;
	 
	}else if(THOST_FTDC_PD_Short == pInvestorPosition->PosiDirection)
	{
		direction = Stype::Sellshort;
	 
	}
	if(pInvestorPosition!=NULL&&pInvestorPosition->Position!=0)
	{
		

		 if(THOST_FTDC_PD_Long == pInvestorPosition->PosiDirection)
		 {
			 direction = Stype::Buy;
			 totalVolum = pInvestorPosition->Position;
			 todayVolum = pInvestorPosition->TodayPosition;
			 avgPrice = pInvestorPosition->PositionCost/pInvestorPosition->Position/getVolumeMultiple(symbol);
		 }else if(THOST_FTDC_PD_Short == pInvestorPosition->PosiDirection)
		 {
			 direction = Stype::Sellshort;
			 totalVolum = pInvestorPosition->Position;
			 todayVolum = pInvestorPosition->TodayPosition;
			 avgPrice= pInvestorPosition->PositionCost/pInvestorPosition->Position/getVolumeMultiple(symbol);
		 }
		 string symbolExchange = getExchange(symbol);
		 if(direction==Stype::Buy&&strcmp(symbolExchange.c_str(),"SHFE")==0)
		 {
			 map<string,int>::iterator it = todayBuyPositionMap.find(symbol);
			 if(it!=todayBuyPositionMap.end())
			 {
				 todayVolum = it->second+todayVolum;
				 //todayBuyPositionMap.insert(pair<string,int>(symbol,todayVolum));
				 it->second = todayVolum;
			 }else
			 {
				 todayBuyPositionMap.insert(pair<string,int>(symbol,todayVolum));
			 }
			  
		 }else if(direction==Stype::Sellshort&&strcmp(symbolExchange.c_str(),"SHFE")==0)
		 {
			 map<string,int>::iterator it = todaySellShortPositionMap.find(symbol);
			 if(it!=todaySellShortPositionMap.end())
			 {
				 todayVolum = it->second+todayVolum;
				 it->second = todayVolum;
			 }else
			 {
				 todaySellShortPositionMap.insert(pair<string,int>(symbol,todayVolum));
			 }

		 }
//		er.setSec(symbol);
//		er.setTradeVol(totalVolum);
//		er.setTradePrice(avgPrice);
//		er.setType(direction);
	}
	if (pInvestorPosition!=NULL&&bIsLast&&pInvestorPosition->Position!=0)
	{
		//this->cth1->
///		this->cth1->OnRspPosition(symbol,direction,totalVolum,avgPrice,1);
	}else if(pInvestorPosition!=NULL&&pInvestorPosition->Position!=0)
	{
///		this->cth1->OnRspPosition(symbol,direction,totalVolum,avgPrice,0);

	}else if(bIsLast)
	{
///		this->cth1->OnRspPosition(symbol,direction,totalVolum,avgPrice,1);
	}
}
/*------------------------------------------------------------------------------
Name:
	OnFrontDisconnected
Parm:
	int nReason
Return:

Description
	The call back function called when the program disconnects from the CTP server
------------------------------------------------------------------------------*/
void CTraderSpi:: OnFrontDisconnected(int nReason)
{
//	this->cth1->setLogonStatus(false);
	//this->Login();
}
/*------------------------------------------------------------------------------
Name:
	OnHeartBeatWarning
Parm:
	int nTimeLapse
Return:

Description

------------------------------------------------------------------------------*/
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	//cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	//cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}
/*------------------------------------------------------------------------------
Name:
	OnRspError
Parm:
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description

------------------------------------------------------------------------------*/
void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}
/*------------------------------------------------------------------------------
Name:
	ReqQryInvestorPositionDetail
Parm:

Return:

Description
	The function that requests the position detail information from CTP
------------------------------------------------------------------------------*/
void CTraderSpi::ReqQryInvestorPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	//strcpy(req.InstrumentID, "IF1112");
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPositionDetail(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
		//	cerr << "--->>> �����ѯͶ���ֲ߳���ϸ: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			 
			break;
		}
		else
		{
		//	cerr << "--->>> �����ѯͶ���ֲ߳���ϸ: "  << iResult << ", �ܵ�����" << endl;
			 
			Sleep(1000);
		}
	} 
	/*this->APIctrl->setPositionDetialFlag(FALSE);
	this->APIctrl->clearPositionDetail();*/
}
/*------------------------------------------------------------------------------
Name:
	OnRspQryInvestorPositionDetail
Parm:
	CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function of the position detail request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail,CThostFtdcRspInfoField *pRspInfo,int nRequestID,bool bIsLast)
{
	if (pInvestorPositionDetail!=NULL)
	{
		//this->APIctrl->UpdatePositionDetail(*pInvestorPositionDetail);
	}
	if (bIsLast)
	{
		//this->APIctrl->setPositionDetialFlag(TRUE);
		//isAvailable = TRUE;
	}
	
}
/*------------------------------------------------------------------------------
Name:
	ReqQryTrade
Parm:

Return:

Description
	The function that requests the trade records from the CTP
------------------------------------------------------------------------------*/
void CTraderSpi::ReqQryTrade()
{
	CThostFtdcQryTradeField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTrade(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			break;
		}
		else
		{
			Sleep(1000);
		}
	} 
}
/*------------------------------------------------------------------------------
Name:
	OnRspQryTrade
Parm:
	CThostFtdcTradeField *pTrade
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function of the trade records request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	//cerr << "--->>> " << "OnRspQryTrade"  << endl;
	if (pTrade!=NULL)
	{
		 
	}
	if (bIsLast)
	{ 
		//isAvailable = TRUE;
	}
	
}
/*------------------------------------------------------------------------------
Name:
	ReqQryOrder
Parm:

Return:

Description
	The function that requests the order records from the CTP
------------------------------------------------------------------------------*/
void CTraderSpi::ReqQryOrder()
{
	CThostFtdcQryOrderField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryOrder(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
		//	cerr << "--->>> ������ѯ: "  << iResult << ((iResult == 0) ? ", �ɹ�" :", ʧ��") <<endl;
			//mdlg->WriteLog("ReqQryOrder �������˳�");
			break;
		}
		else
		{
 
			Sleep(1000);
		}
	} 
	 
}
/*------------------------------------------------------------------------------
Name:
	OnRspQryOrder
Parm:
	CThostFtdcOrderField *pOrder
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function for order record request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pOrder!=NULL)
	{
		if (pOrder->OrderStatus == '3'||pOrder->OrderStatus == '1')
		{
			//APIctrl->UpdateOrderMessage(*pOrder);
		}
	}
	if (bIsLast)
	{
		//this->APIctrl->setOrderFlag(TRUE);
		ReqQryInvestorPosition();
	}
}


void CTraderSpi::sendOrder(Order cor1)
{
	CThostFtdcInputOrderField req; 
	memset(&req, 0, sizeof(req)); 
	//	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID); 
	//	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID); 
	//	///��Լ����
	strcpy(req.InstrumentID, cor1.getSec().c_str()); 
	//	///��������
	int idtmp = cor1.orderRef; 
	sprintf(ORDER_REF, "%d", idtmp); 
	strcpy(req.OrderRef, ORDER_REF); 

	///�����۸�����: �޼�
	if(cor1.getSendPrice()==0)
	{
		req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}else
	{
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}
	req.LimitPrice = cor1.getSendPrice();
	///��������: 
	 
	if(cor1.getType()==Stype::Buy)
	{
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		req.Direction = THOST_FTDC_D_Buy;
		
	}else if(cor1.getType()==Stype::Buytocover)
	{
		//req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;//for test only ,you need to change it in real
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		string tmpEx = getExchange(cor1.getSec());
		if(strcmp(tmpEx.c_str(),"SHFE")==0)
		{
			req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		}else
		{
			req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		}
		req.Direction = THOST_FTDC_D_Buy;
	}else if(cor1.getType()==Stype::Sellshort)
	{
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		req.Direction = THOST_FTDC_D_Sell;
	}else
	{
		//req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;//for test only ,you need to change it in real
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		//req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		req.Direction = THOST_FTDC_D_Sell;
		string tmpEx = getExchange(cor1.getSec());
		if(strcmp(tmpEx.c_str(),"SHFE")==0)
		{
			//for SHFE close or close today
			req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		}else
		{
			//for others ,only close
			req.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		}
	}
	///��Ͽ�ƽ��־: ����
	//req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = cor1.getSendPrice();
	///����: 1
	req.VolumeTotalOriginal = cor1.getSendVol();
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
 
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;

	///�û�ǿ����־: ��
	req.UserForceClose = 0;
	//req.user
	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	cerr << "--->>> ����¼������: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;

}
/*------------------------------------------------------------------------------
Name:
	ReqInstrument
Parm:
	
Return:

Description
	The function that requests all the instruments information of all
	the Exchanges
------------------------------------------------------------------------------*/
void CTraderSpi::ReqQryInstrument()
{
	//mdlg->WriteLog("����");
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	instrumentDataBase.clear();
	int itmp = 0;
	while (true)
	{
		int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			break;
		}
		else
		{	
			Sleep(1000);
		}
	}
}
/*------------------------------------------------------------------------------
Name:
	OnRspQryInstrument
Parm:
	CThostFtdcInstrumentField *pInstrument
	CThostFtdcRspInfoField *pRspInfo
	int nRequestID
	bool bIsLast
Return:

Description
	The call back function of the Instrument query request
------------------------------------------------------------------------------*/
void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << "OnRspQryInstrument" << endl;
	if (!IsErrorRspInfo(pRspInfo))
	{
		///�����ѯ��Լ
		if (pInstrument!=NULL)
		{
			ISMAPFILLED = TRUE;
			instrumentDataBase.insert(pair<string,	CThostFtdcInstrumentField>(pInstrument->InstrumentID,*pInstrument));
		}
		if (bIsLast)
		{
			ISMAPFILLEDFINISHED = TRUE;
///			this->cth1->setLogonStatus(true);
		}
	}
}
void CTraderSpi::OnRspQryTradingCode(struct CThostFtdcTradingCodeField *,struct CThostFtdcRspInfoField *,int,bool)
{

}
void  CTraderSpi::OnRspQryInstrumentCommissionRate(struct CThostFtdcInstrumentCommissionRateField *,struct CThostFtdcRspInfoField *,int,bool)
{

}
void CTraderSpi::OnRspQryInstrumentMarginRate(struct CThostFtdcInstrumentMarginRateField *,struct CThostFtdcRspInfoField *,int,bool)
{

}
void CTraderSpi::OnRspUserPasswordUpdate(struct CThostFtdcUserPasswordUpdateField *,struct CThostFtdcRspInfoField *,int,bool)
{

}
 
void CTraderSpi::OnRspQrySettlementInfo(struct CThostFtdcSettlementInfoField *,struct CThostFtdcRspInfoField *,int,bool)
{

}
void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pTradingAccount!=NULL)
	{
		double avavl = pTradingAccount->Available;
		double quanyi = pTradingAccount->CurrMargin+avavl;
///		this->cth1->OnRspAccount(quanyi,avavl);
	
	}

}

void CTraderSpi::setUserName(string name1)
{
	strcpy(this->INVESTOR_ID,name1.c_str());
}


void CTraderSpi::setPassword(string password1)
{
	strcpy(this->PASSWORD,password1.c_str());
}

void CTraderSpi::setBrokerID(string brokerid1)
{
	strcpy(this->BROKER_ID,brokerid1.c_str());
}


void CTraderSpi::Login(void)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);//�ڴ˴��޸�BROKER_ID
	strcpy(req.UserID, this->INVESTOR_ID);//�ڴ˴��޸�INVESTOR_ID
	strcpy(req.Password, this->PASSWORD);//�ڴ˴��޸�PASSWORD
	//strcpy(INVESTOR_ID,account);
	//strcpy(PASSWORD,password);
	strcpy(req.InterfaceProductInfo,"TestInfo");
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> Trader�����û���¼����: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
}
int CTraderSpi::getVolumeMultiple(string symbolname)
{
	map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.find(symbolname);
	if(it !=instrumentDataBase.end())
	{
		return it->second.VolumeMultiple;
	}else
	{
		return 0;
	}
}
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if(!isMyOrder(pOrder->SessionID,pOrder->FrontID))
	{
		cerr << "Not my order" << endl;
		return;
	}
	int orderreftmp = atoi(pOrder->OrderRef);//COrder cord1;
	orderSysMap.insert(pair<int,string>(orderreftmp,pOrder->OrderSysID));
	int orderstatus ;
	if(pOrder->OrderSubmitStatus ==THOST_FTDC_OSS_InsertRejected)
	{
		orderstatus = Order::Reject;
		 
	}else if(pOrder->OrderStatus==THOST_FTDC_OST_Unknown)
	{
		orderstatus = Order::Default;
	}else if(pOrder->OrderStatus==THOST_FTDC_OST_AllTraded)
	{
		orderstatus = Order::Filled;
		 
	}else if(pOrder->OrderStatus==THOST_FTDC_OST_PartTradedQueueing)
	{
		orderstatus = Order::PartialFill;

	}else if(pOrder->OrderStatus==THOST_FTDC_OST_Canceled)
	{
		orderstatus = Order::Cancelled;
	}else
	{
		 orderstatus = -1;
	}
///	this->cth1->updateOrderStatus(orderreftmp,orderstatus);
}

	///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
//	ExecReport ec ;
//	ec.orderRef = atoi(pTrade->OrderRef);
//	ec.setTradePrice(pTrade->Price);
//	ec.setSec(pTrade->InstrumentID);
//	ec.setTradeVol(pTrade->Volume);
//	ec.setTradetime(pTrade->TradeTime);
//	if(pTrade->Direction==THOST_FTDC_D_Sell)
//	{
//		if(pTrade->OffsetFlag==THOST_FTDC_OF_Open)
//		{
//			ec.type = Stype::Sellshort;
//		}else
//		{
//			ec.type= Stype::Sell;
//		}
//	}else if(pTrade->Direction==THOST_FTDC_D_Buy)
//	{
//		if(pTrade->OffsetFlag==THOST_FTDC_OF_Open)
//		{
//			ec.type = Stype::Buy;
//		}else
//		{
//			ec.type = Stype::Buytocover;
//		}
//	}
//
//	string tmpSymbol = pTrade->InstrumentID;

///	this->cth1->updateOrderTrade(ec);
}
void CTraderSpi::OnErrRtnOrderInsert(struct CThostFtdcInputOrderField *tmp,struct CThostFtdcRspInfoField *tmp1)
{
	cerr << "--->>> �����û���¼����: " << endl;
}

void CTraderSpi::CancelOrder(Order cord1)
{
	 

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID,this->BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, this->INVESTOR_ID);
	///������������
//	 
	int idtmp = cord1.orderRef;
	sprintf(ORDER_REF, "%d", idtmp);
	strcpy(req.OrderRef, ORDER_REF);
    string orderSysTMp = "";
	if(this->orderSysMap.find(idtmp)!=orderSysMap.end())
	{
	
		orderSysTMp  = this->orderSysMap.find(idtmp)->second;
	}
	strcpy(req.OrderSysID,orderSysTMp.c_str());
	strcpy(req.ExchangeID,(this->getExchange(cord1.getSec())).c_str());

	///ǰ�ñ��
	req.FrontID = FRONT_ID;
	///�Ự���
	req.SessionID = SESSION_ID;
 
	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
 
	///��Լ����
	strcpy(req.InstrumentID, cord1.sec.c_str());
	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
	cerr << "--->>> ������������: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
	//ORDER_ACTION_SENT = true;
}
void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << "OnRspOrderAction" << endl;
	//COrder cord1;
	//cord1.setorderID(atoi(pInputOrderAction->OrderRef));
	//cord1.setOrderStatus(COrderStatus::Canceled);
	//IsErrorRspInfo(pRspInfo);
	int i = 1;
}



bool CTraderSpi::isMyOrder(TThostFtdcSessionIDType sID, TThostFtdcFrontIDType fID)
{
	map<TThostFtdcSessionIDType,TThostFtdcFrontIDType>::iterator it  = sessionInFoMap.find(sID);
	if(it!=sessionInFoMap.end())
	{
		if( it->second==fID)
		{
			return true;
		}
	}
	//sessionInFoMap
	return false;
}


double CTraderSpi::getMinTick(string symbolname)
{
	map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.find(symbolname);
	if(it !=instrumentDataBase.end())
	{
		return it->second.PriceTick;
	}else
	{
		return 0;
	}
	return 0;
}


string CTraderSpi::getExchange(string symbolname)
{
	map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.find(symbolname);
	if(it !=instrumentDataBase.end())
	{
		return it->second.ExchangeID;
	}else
	{
		return NULL;
	}
	return NULL;
}


double CTraderSpi::getUpPrice(string symbolname1)
{
	map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.find(symbolname1);
	if(it !=instrumentDataBase.end())
	{
		//return it->second.;
	}
	return 0;
}


double CTraderSpi::getLowPrice(string symbolname1)
{
	return 0;
}

// clear today Position when the new day came, should be called manually
void CTraderSpi::clearTodayPosition(void)
{
	this->todayBuyPositionMap.clear();
	this->todaySellShortPositionMap.clear();
}
list<string> CTraderSpi::screenContracts(string productName)
{
	list<string> result;
	for(map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.begin(); it != instrumentDataBase.end(); it++ )
	{
		string contractName = it->first;
		if(contractName.find(productName)==0)
		{
			//add by aaron 2013-4-8 efp contract is useless,like cuefp.
			if(contractName.substr(contractName.length()-3,3).compare("efp")!=0)
			{
		           result.push_back(contractName);
			}
			
		}
		
	}
	return result;
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			//cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			//mdlg->WriteLog("ReqQryTradingAccount �������Ƴ�");
			break;
		}
		else
		{
			//cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ", �ܵ�����" << endl;
			//if(!APIctrl->isThisAvalable)
			//{
				//return;
			//}
			//mdlg->WriteLog("ReqQryTradingAccount����ѭ��");
			Sleep(1000);
		}
	} 
}

void CTraderSpi::requestInstrument()
{
//	AfxBeginThread(SendInstrumentReplyhreadFunc,this);//�˴�����ʹ��createthread���棬afxΪMFC�߳�ģ��
}
// iflag = 0 not the last; iflag =1 the last
void CTraderSpi::replyInstrumentRequest()
{
	int i =1;
	if(instrumentDataBase.size()!=0)
	for(map<string,CThostFtdcInstrumentField>::iterator it = instrumentDataBase.begin(); it != instrumentDataBase.end(); it++ )
	{
		if(i!=instrumentDataBase.size())
		{
			//this->cth1->updateOrderTrade 
///			this->cth1->OnRspInstrument( it->first,0);
		}else
		{
///			this->cth1->OnRspInstrument( it->first,1);
		}
		i++;
	}
}