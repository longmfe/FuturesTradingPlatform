/*------------------------------------------------------------------------------
Copyright file="MdSpi.h" Company="CiticNewedge
Copyright (c) 2011 CiticNewedge. All rights reserved.
Author:刘钦涛
Mail:  liuqt@citicnewedge.com
Date:  2011年8月24日
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
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	
	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
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