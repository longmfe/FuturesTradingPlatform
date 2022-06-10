/*------------------------------------------------------------------------------
Copyright file="TraderSpi.h" Company="CiticNewedge
Copyright (c) 2011 CiticNewedge. All rights reserved.
Author:������
Mail:  liuqt@citicnewedge.com
Date:  2011��8��24��
Description
	The class that sends and receives data from CTP
------------------------------------------------------------------------------*/
#pragma once
#include "ThostTraderApi\ThostFtdcTraderApi.h"
#include <list>
#include <map>
#include <string>
#include "Order.h"
using namespace std;

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;
	///
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ɽ�֪ͨ
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ��Ӧ
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	virtual void OnHeartBeatWarning(int nTimeLapse);


	///�ֲ���ϸ��ѯ��Ӧ
	virtual void  OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//�ɽ���ѯ��Ӧ
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//������ѯ��Ӧ
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//���㵥��ѯ��Ӧ
	virtual void OnRspQrySettlementInfo( 
		CThostFtdcSettlementInfoField *pSettlementInfo,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	///�޸�������Ӧ
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, 
													CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯ��Լ��֤������Ӧ
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, 
													CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///�����ѯ��Լ����������Ӧ
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
													CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryTradingCode( 
		CThostFtdcTradingCodeField *pTradingCode,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
private:
	TThostFtdcBrokerIDType BROKER_ID;
	TThostFtdcInvestorIDType INVESTOR_ID;
	TThostFtdcPasswordType PASSWORD;
	CThostFtdcTraderApi* pUserApi;
	int iRequestID;
	 
	// �Ự����
	TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
	TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
	TThostFtdcOrderRefType	ORDER_REF;	//��������
	 
	map<string,CThostFtdcInstrumentField> instrumentDataBase;
	map<TThostFtdcSessionIDType,TThostFtdcFrontIDType> sessionInFoMap;//ͨ��SessionID ��Front ID ����𷢵��Ƿ�Ϊ��ǰ������ĵ���
	map<string,int> todayBuyPositionMap;//��¼�ͻ�������ֲ�
	map<string,int> todaySellShortPositionMap;//��¼�ͻ��������ֲ�
	int iTimeout;
	bool ISMAPFILLED;
	bool ISMAPFILLEDFINISHED;

	string currentDate;
	int isInitialInstrumentRequest;// 
	map<int,string> orderSysMap;
public:


	//CTraderSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcTraderApi *ppUserapi,APIController* pAPI);
	CTraderSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcTraderApi *ppUserapi);
	///�û���¼����
	void ReqUserLogin(TThostFtdcInvestorIDType account,TThostFtdcPasswordType password);
	///Ͷ���߽�����ȷ��
	void ReqSettlementInfoConfirm();
	///�����ѯ��Լ
	void ReqQryInstrument();

	//added by aaronwz 2013-3-6
	void SearchContractByProduct();

	///�����ѯ�ʽ��˻�
	void ReqQryTradingAccount();
	///�����ѯͶ���ֲ߳�
	void ReqQryInvestorPosition();
 
	// �Ƿ��յ��ɹ�����Ӧ
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// �Ƿ��ҵı����ر�
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// �Ƿ����ڽ��׵ı���
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

	//�����ѯͶ���ֲ߳���ϸ
	void ReqQryInvestorPositionDetail();
	//�ɽ�����ѯ
	void ReqQryTrade();
	//���󱨵���ѯ
	void ReqQryOrder();
 
	void ReqQryTradingCode();

	void ReqQryInvestorPosition(string symbolname);
 
	void sendOrder(Order cor1);
	void setUserName(string name1);
	void setPassword(string password1);
	void setBrokerID(string brokerid1);
	void Login(void);
	int getVolumeMultiple(string symbolname);
	void CancelOrder(Order cord1);
	bool isMyOrder(TThostFtdcSessionIDType sID, TThostFtdcFrontIDType fID);
	double getMinTick(string symbolname);
	string getExchange(string symbolname);
	double getUpPrice(string symbolname1);
	double getLowPrice(string symbolname1);

	void clearTodayPosition(void);

	//Add by aaron
	list<string> screenContracts(string productName);
	void requestInstrument();// 
	void replyInstrumentRequest();
	//TradeApplication *cth1;


};
