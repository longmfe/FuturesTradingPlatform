/*------------------------------------------------------------------------------
Copyright file="TraderSpi.h" Company="CiticNewedge
Copyright (c) 2011 CiticNewedge. All rights reserved.
Author:刘钦涛
Mail:  liuqt@citicnewedge.com
Date:  2011年8月24日
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

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);


	///持仓明细查询响应
	virtual void  OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//成交查询响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//报单查询响应
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	//结算单查询响应
	virtual void OnRspQrySettlementInfo( 
		CThostFtdcSettlementInfoField *pSettlementInfo,   
		CThostFtdcRspInfoField *pRspInfo,   
		int nRequestID,   
		bool bIsLast);
	///修改密码响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, 
													CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, 
													CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///请求查询合约手续费率响应
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
	 
	// 会话参数
	TThostFtdcFrontIDType	FRONT_ID;	//前置编号
	TThostFtdcSessionIDType	SESSION_ID;	//会话编号
	TThostFtdcOrderRefType	ORDER_REF;	//报单引用
	 
	map<string,CThostFtdcInstrumentField> instrumentDataBase;
	map<TThostFtdcSessionIDType,TThostFtdcFrontIDType> sessionInFoMap;//通过SessionID 和Front ID 来辨别发单是否为当前软件发的单子
	map<string,int> todayBuyPositionMap;//记录客户本日买持仓
	map<string,int> todaySellShortPositionMap;//记录客户本日卖持仓
	int iTimeout;
	bool ISMAPFILLED;
	bool ISMAPFILLEDFINISHED;

	string currentDate;
	int isInitialInstrumentRequest;// 
	map<int,string> orderSysMap;
public:


	//CTraderSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcTraderApi *ppUserapi,APIController* pAPI);
	CTraderSpi(TThostFtdcBrokerIDType brokerID,CThostFtdcTraderApi *ppUserapi);
	///用户登录请求
	void ReqUserLogin(TThostFtdcInvestorIDType account,TThostFtdcPasswordType password);
	///投资者结算结果确认
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument();

	//added by aaronwz 2013-3-6
	void SearchContractByProduct();

	///请求查询资金账户
	void ReqQryTradingAccount();
	///请求查询投资者持仓
	void ReqQryInvestorPosition();
 
	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// 是否正在交易的报单
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

	//请求查询投资者持仓详细
	void ReqQryInvestorPositionDetail();
	//成交单查询
	void ReqQryTrade();
	//请求报单查询
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
