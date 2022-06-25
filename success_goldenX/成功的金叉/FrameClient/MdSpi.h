/*------------------------------------------------------------------------------
Copyright file="MdSpi.h" Company="CiticNewedge
Copyright (c) 2011 CiticNewedge. All rights reserved.
Author:������
Mail:  liuqt@citicnewedge.com
Date:  2011��8��24��
Description
	The class that connects to CTP server to get Market data
------------------------------------------------------------------------------*/
#pragma once
#include "ThostTraderApi\ThostFtdcMdApi.h"
#include "MarketApplication.h"
#include <map>
using namespace std;
class MarketApplication;
class CMdSpi : public CThostFtdcMdSpi
{
public:
	CMdSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcMdApi *ppUserapi,MarketApplication *cctphandle);
public:
	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
		
	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	
	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

public:
	void ReqUserLogin(TThostFtdcInvestorIDType account,TThostFtdcPasswordType password);
	void SubscribeMarketData();
	void SubscribeMarketData(string symbol);
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
private:
	TThostFtdcBrokerIDType BROKER_ID;
	TThostFtdcInvestorIDType INVESTOR_ID;
	TThostFtdcPasswordType PASSWORD;
	CThostFtdcMdApi* pUserApi;
	int iRequestID;
	char** ppInstrumentID;
	int iInstrumentID;
	MarketApplication *ctp;
	int preVolum;
	int prepreVolum;
	map<string,string> marketSymbolMap;
	map<string,int> marketPreVolum;
	map<string,int> marketPrePreVolum;

public:
	 
	void setUserName(string name);
	void setPassword(string password);
	void Login(void);
	void autoRequestMarket();
};