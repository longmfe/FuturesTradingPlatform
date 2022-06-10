#pragma once
#include "ThostFtdcTraderApi.h"
#include"Config.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
enum OrderType {
	_OT_Normal = 0,
	_OT_FAK = 1,
	_OT_FOK = 2,
};
enum OrderDirectionType {
	_DT_Null = 0,
	_DT_Buy = '0',
	_DT_Sell = '1'
};

enum OrderOffsetFlagType {
	_OF_Open = '0',
	///平仓
	_OF_Close = '1',
	///强平
	_OF_ForceClose = '2',
	///平今
	_OF_CloseToday = '3',
	///平昨
	_OF_CloseYesterday = '4',
};

enum OrderHedgeFlagType {
	///投机
	_CHF_Speculation = '1',
	///套利
	_CHF_Arbitrage = '2',
	///套保
	_CHF_Hedge = '3',
	///做市商
	_CHF_MarketMaker = '4',
	///匹配所有的值
	_CHF_AllValue = '9',
};
struct SOrderParam {
	const char* instrumentId;
	const char* exchangeId;
	OrderType orderType;
	OrderDirectionType direction;
	OrderOffsetFlagType offset;
	OrderHedgeFlagType hedgeType;
	int volume;
	double price;
};

class CUser : public CThostFtdcTraderSpi
{
public:
	CUser() {}
	~CUser() {}
	const char* GetDirectionDsp(TThostFtdcDirectionType dir)
	{
		switch (dir)
		{
		case THOST_FTDC_D_Buy:
			return ("买");
			break;
		case THOST_FTDC_D_Sell:
			return ("卖");
			break;
		default:
			return "";
		}
	}

	const char* GetPosDirectionDsp(TThostFtdcPosiDirectionType dir) {
		switch (dir)
		{
		case THOST_FTDC_PD_Net:
			return ("净");
			break;
		case THOST_FTDC_PD_Long:
			return ("多");
			break;
		case THOST_FTDC_PD_Short:
			return ("空");
			break;
		default:
			return "";
		}
	}

	const char* GetOffsetFlagDsp(TThostFtdcOffsetFlagType flg)
	{
		switch (flg)
		{
		case THOST_FTDC_OF_Open:
			return ("开");
			break;
		case THOST_FTDC_OF_Close:
			return ("平");
			break;
		case THOST_FTDC_OF_ForceClose:
			return ("强平");
			break;
		case THOST_FTDC_OF_CloseToday:
			return ("平今");
			break;
		case THOST_FTDC_OF_CloseYesterday:
			return ("平昨");
			break;
		default:
			return "";
		}
	}

	const char* GetStatusDsp(TThostFtdcOrderStatusType sta)
	{
		switch (sta)
		{
		case THOST_FTDC_OST_AllTraded:
			return "全部成交";
			break;
		case THOST_FTDC_OST_PartTradedQueueing:
			return ("部分成交还在队列中");
			break;
		case THOST_FTDC_OST_PartTradedNotQueueing:
			return ("部分成交不在队列中");
			break;
		case THOST_FTDC_OST_NoTradeQueueing:
			return ("未成交还在队列中");
			break;
		case THOST_FTDC_OST_NoTradeNotQueueing:
			return ("未成交不在队列中");
			break;
		case THOST_FTDC_OST_Canceled:
			return ("撤单");
			break;
		case THOST_FTDC_OST_Unknown:
			return ("未知单");
			break;
		default:
			return "未知状态";
		}
	}

	const char* GetHedgeDsp(TThostFtdcHedgeFlagType hedge) {
		switch (hedge) {
		case THOST_FTDC_HF_Speculation:
			return "投机";
			break;
		case THOST_FTDC_HF_Arbitrage:
			return "套利";
			break;
		case THOST_FTDC_HF_Hedge:
			return "套保";
			break;
		case THOST_FTDC_HF_MarketMaker:
			return "做市";
			break;
		default:
			return "未知套保类型";
		}
	}

	const char* GetRtnCodeDsp(int code) {
		switch (code) {
		case 0:
			return "成功";
			break;
		case -1:
			return "网络连接失败";
			break;
		case -2:
			return "未处理请求超过许可数";
			break;
		case -3:
			return "每秒发送请求数超过许可数";
			break;
		default:
			return "未知错误";
		}
	}
	int Init()
	{
		m_pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("");
		m_pUserApi->RegisterSpi(this);

		m_pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);

		// register public stream
		m_pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
		
		char trader_server_address[128] = { 0 };

		// real-time data
		
		strcpy(trader_server_address, TRADER_ADDRESS.data());

		printf("connecting to %s\n", trader_server_address);
		m_pUserApi->RegisterFront(trader_server_address);
		m_pUserApi->Init();
		//m_pUserApi->Join();
		return 0;
	}
	///在API连接后发起认证
	virtual void OnFrontConnected() {
		cout << ":OnFrontConnected_交易前置连接" << endl;
		static const char *version = m_pUserApi->GetApiVersion();
		cout << "------交易当前版本号 ：" << version << " ------" << endl;
		//cout << "pls input authid and authcode" << endl;
		//cin>>
		ReqAuthenticate();
	}
	int ReqAuthenticate()
	{
		
		CThostFtdcReqAuthenticateField field;
		memset(&field, 0, sizeof(field));
		strncpy(field.BrokerID, BROKERID.data(), sizeof(field.BrokerID));
		strncpy(field.UserID, USERID.data(), sizeof(field.UserID));
		strncpy(field.AppID, APPID.data(), sizeof(field.AppID));
		strncpy(field.AuthCode, AUTHCODE.data(), sizeof(field.AuthCode));
		int re= m_pUserApi->ReqAuthenticate(&field, ++RequestID);
		if (re) {
			cout << ": send ReqAuthenticate fail: " << re << "," << endl;
		}
		//else cout << ": send ReqAuthenticate success: " << re << "," << endl;
		return re;
	}

	int ReqUserLogin()
	{
		printf("====ReqUserLogin====,用户登录中...%s, %s\n\n", USERID.data(), PASSWORD.data());
		CThostFtdcReqUserLoginField reqUserLogin;
		memset(&reqUserLogin, 0, sizeof(reqUserLogin));
		strncpy(reqUserLogin.BrokerID, BROKERID.data(), sizeof(reqUserLogin.BrokerID));
		strncpy(reqUserLogin.UserID, USERID.data(), sizeof(reqUserLogin.UserID));
		strncpy(reqUserLogin.Password, PASSWORD.data(), sizeof(reqUserLogin.Password));
		strncpy(reqUserLogin.TradingDay, "20190524", sizeof(reqUserLogin.TradingDay));
		int re=m_pUserApi->ReqUserLogin(&reqUserLogin, ++RequestID);
		if (re) {
			cout << ": send ReqUserLogin fail: " << re << "," << endl;
		}
		//else cout << ": send ReqUserLogin success: " << re << "," << endl;
		return re;
	}
	int ReqUserLogout()
	{
		//printf("====ReqUserLogout====,用户登出中...\n\n");
		CThostFtdcUserLogoutField reqLogout;
		strncpy(reqLogout.BrokerID, BROKERID.data(), sizeof(reqLogout.BrokerID));
		strncpy(reqLogout.UserID, USERID.data(), sizeof(reqLogout.UserID));
		int re = m_pUserApi->ReqUserLogout(&reqLogout, ++RequestID);
		if (re) {
			cout << ": send ReqUserLogout fail: " << re << "," << endl;
		}
		return re;
	}
	int PutOrder(SOrderParam* param) {
		cout << "::PutOrder" << "\t";
		CThostFtdcInputOrderField req;
		memset(&req, 0, sizeof(req));

		strncpy(req.InstrumentID, param->instrumentId, sizeof(req.InstrumentID));
		req.CombHedgeFlag[0] = param->hedgeType;
		req.CombOffsetFlag[0] = param->offset;
		req.VolumeTotalOriginal = param->volume;
		strncpy(req.ExchangeID, param->exchangeId, sizeof(req.ExchangeID));
		req.Direction = param->direction;
		req.LimitPrice = param->price;
		switch (param->orderType) {
		case _OT_FAK:
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.TimeCondition = THOST_FTDC_TC_IOC;
			///成交量类型: 任何数量
			req.VolumeCondition = THOST_FTDC_VC_AV;
			///最小成交量: 1
			req.MinVolume = 1;
			break;
		case _OT_FOK:
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			req.TimeCondition = THOST_FTDC_TC_IOC;
			///成交量类型: 任何数量
			req.VolumeCondition = THOST_FTDC_VC_CV;
			///最小成交量: 1
			req.MinVolume = 1;
			break;
		default:
			if (req.LimitPrice != 0) {
				req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				req.TimeCondition = THOST_FTDC_TC_GFD;
			}
			else {
				req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
				req.TimeCondition = THOST_FTDC_TC_IOC;
			}
			///成交量类型: 任何数量
			req.VolumeCondition = THOST_FTDC_VC_AV;
			///最小成交量: 1
			req.MinVolume = 1;
		}
		///触发条件: 立即
		req.ContingentCondition = THOST_FTDC_CC_Immediately;
		///强平原因: 非强平
		req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///自动挂起标志: 否
		req.IsAutoSuspend = 0;
		///用户强评标志: 否
		req.UserForceClose = 0;

		///经纪公司代码
		strncpy(req.BrokerID, BROKERID.data(), sizeof(req.BrokerID));

		///投资者代码
		strncpy(req.InvestorID, INVESTORID.data(), sizeof(req.InvestorID));

		strncpy(req.UserID, USERID.data(), sizeof(req.UserID));

		///报单引用
		sprintf(req.OrderRef, "%09lld", m_nextOrderRef++);//相当于本地单号，不要重复，在此用递增的数字表示

	
		int re = m_pUserApi->ReqOrderInsert(&req, ++RequestID);
		if (re) {
			cout << ": send ReqOrderInsert fail: " << re << ","  << endl;
		}
		cout << req.InstrumentID << "|" << req.Direction << "|";
		cout << req.LimitPrice << "|" << (long)req.VolumeTotalOriginal << "|";
		cout<<req.CombOffsetFlag[0] << "|发送下单前后|\t" ;
		cout << endl;
		return re;
	}
	//撤单请求
	int CancelOrder(const char* orderid, const char* instrumentid, const char* exchangeid) {
		if (orderid) {
			CThostFtdcInputOrderActionField req;
			memset(&req, 0, sizeof(req));
			req.ActionFlag = THOST_FTDC_AF_Delete;
			strncpy(req.BrokerID, BROKERID.data(), sizeof(req.BrokerID));
			strncpy(req.InvestorID, INVESTORID.data(), sizeof(req.InvestorID));
			strncpy(req.UserID, USERID.data(), sizeof(req.UserID));
			strncpy(req.InstrumentID, instrumentid, sizeof(req.InstrumentID));
			strncpy(req.OrderSysID, orderid, sizeof(req.OrderSysID));
			strncpy(req.ExchangeID, exchangeid, sizeof(req.ExchangeID));
			int re = m_pUserApi->ReqOrderAction(&req, ++RequestID);
			if (re) {
				cout << ": send ReqOrderAction fail: " << re << "," << GetRtnCodeDsp(re) << endl;
			}
			cout << instrumentid << "|" << exchangeid << "|" << orderid  << "|发起撤单\t" << endl;
		}
		return 0;
	}
	//结算确认
	int ConfirmSett(const char* sDate=NULL) {
		CThostFtdcSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, BROKERID.data(), sizeof(req.BrokerID));
		strncpy(req.InvestorID, INVESTORID.data(), sizeof(req.InvestorID));
		if (sDate && sDate[0]) //确认指定日
			strncpy(req.ConfirmDate, sDate, sizeof(req.ConfirmDate));
		int re = m_pUserApi->ReqSettlementInfoConfirm(&req, ++RequestID);
		if (re) {
			cout << ": send ReqSettlementInfoConfirm fail: " << re  << endl;
		}
		return re;
	}

	int QryInst() {
		CThostFtdcQryInstrumentField req = { 0 };
		return m_pUserApi->ReqQryInstrument(&req, 0);
	}

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) 
	{
		std::cout << "OnRspQryInstrument";
		if (pInstrument)
		{
			std::cout << pInstrument->InstrumentID << "\t" << pInstrument->InstrumentName;
		}
		std::cout << std::endl;
	}

	//修改密码
	int ReqChgPwd(const char* oldPass, const char* newPass) {
		CThostFtdcUserPasswordUpdateField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, BROKERID.c_str(), sizeof(req.BrokerID));
		strncpy(req.UserID, USERID.c_str(), sizeof(req.UserID));
		strncpy(req.OldPassword, oldPass, sizeof(req.OldPassword));
		strncpy(req.NewPassword, newPass, sizeof(req.NewPassword));
		int re = m_pUserApi->ReqUserPasswordUpdate(&req, ++RequestID);
		if (re) {
			cout << ": send ReqUserPasswordUpdate fail: " << re << "," << GetRtnCodeDsp(re) << endl;
		}
		return re;
	}

	//查询报单请求
	int QryOrder() {
		CThostFtdcQryOrderField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, BROKERID.data(), sizeof(req.BrokerID));
		strncpy(req.InvestorID, INVESTORID.data(), sizeof(req.InvestorID));
		int re = m_pUserApi->ReqQryOrder(&req, ++RequestID);
		if (re) {
			cout << ": send ReqQryOrder fail: " << re << "," << GetRtnCodeDsp(re) << endl;
		}
		return re;
	}
	//查询成交请求
	int QryTrade() {
		CThostFtdcQryTradeField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, BROKERID.data(), sizeof(req.BrokerID));
		strncpy(req.InvestorID, INVESTORID.data(), sizeof(req.InvestorID));
		int re = m_pUserApi->ReqQryTrade(&req, ++RequestID);
		if (re) {
			cout << ": send ReqQryTrade fail: " << re << "," << GetRtnCodeDsp(re) << endl;
		}
		return re;
	}

	//认证成功后发起登录
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		
		cout << ":OnRspAuthenticate_trader";
		if (pRspInfo != NULL && pRspInfo->ErrorID)
		{
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
			
		}
		else
		{
			cout << ":" << "交易认证成功";
			//ReqUserLogin();
		}
		if (pRspAuthenticateField)
			cout << ":" << pRspAuthenticateField->BrokerID << ":" << pRspAuthenticateField->AppID<< ":" << pRspAuthenticateField->AppType << ":" << pRspAuthenticateField->UserID;
		cout << endl;

	}

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspUserLogin_trader";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "交易登录成功";
			m_bMdLogin = true;
			#ifndef WIN32
			#define _atoi64(val)     strtoll(val, NULL, 10)
			#endif
			m_nextOrderRef = _atoi64(pRspUserLogin->MaxOrderRef) + 1;
			ConfirmSett("20191126");
		}
		if (pRspUserLogin)
			cout << ":" << pRspUserLogin->BrokerID << ":" << pRspUserLogin->SystemName << ":" << pRspUserLogin->UserID;
		cout << endl;

	}

	//登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspUserLogout_trader:";
		if (pRspInfo)
			cout << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		m_bMdLogin = false;
		cout << endl;
	}

    // 报单插入应答
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if (pInputOrder) {
			cout << "::OnRspOrderInsert" << "\t";
			cout << pInputOrder->InstrumentID << "|" << pInputOrder->Direction;
			cout << "|" << pInputOrder->LimitPrice << "|" << (long)pInputOrder->VolumeTotalOriginal << "|" << pInputOrder->CombOffsetFlag[0];
			if (pRspInfo && pRspInfo->ErrorID) {
				cout << "|报单失败:" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
			}
			else {
				cout << "|报单插入成功";
			}
			cout <<"\t"<< endl;
		}
	}

	///请求查询报单响应
	void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{

		if (pRspInfo && pRspInfo->ErrorID) {
			cout<< ":OnRspQryOrder:" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg << endl;
		}
		else if (pOrder) {
			cout<< ":OnRspQryOrder:"  << pOrder->OrderRef
				<< "\t" << pOrder->InstrumentID
				<< " " << GetDirectionDsp(pOrder->Direction)
				<< " " << GetOffsetFlagDsp(pOrder->CombOffsetFlag[0])
				<< " P:" << pOrder->LimitPrice
				<< " V:" << pOrder->VolumeTotal
				<< " " << GetStatusDsp(pOrder->OrderStatus)
				<< " SID:" << pOrder->OrderSysID << "|T:" << pOrder->InsertTime << "|E:" << pOrder->ExchangeID << "|U:" << pOrder->UserID << "|" << pOrder->StatusMsg;
			cout << endl;
			if(pOrder->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
				CancelOrder(pOrder->OrderSysID, pOrder->InstrumentID, pOrder->ExchangeID);
			
		}
		if (bIsLast) {
			cout << "查询报单完毕!\n";
		}

	}

	//报单操作响应
	void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << ":OnRspOrderAction";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "订单操作成功";
		}
		if (pInputOrderAction)
			cout << ":" << pInputOrderAction->OrderSysID << " " << pInputOrderAction->OrderRef << " " << (pInputOrderAction->ActionFlag == THOST_FTDC_AF_Delete ? "THOST_FTDC_AF_Delete" : "THOST_FTDC_AF_Modify");
		cout << endl;
	}

	//用户密码修改应答
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << ":OnRspUserPasswordUpdate" << endl;
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << "ErrorId:" << pRspInfo->ErrorID << "\tErrorInfo:" << pRspInfo->ErrorMsg;
		}
		else if (pUserPasswordUpdate) {
			cout << pUserPasswordUpdate->UserID << "  ";
			cout << pUserPasswordUpdate->BrokerID << "  ";
			cout << pUserPasswordUpdate->OldPassword << "  ";
			cout << pUserPasswordUpdate->NewPassword;
		}
		cout << endl;
	}

	//查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << ":OnRspQryTrade"<<"\t";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else if (pTrade) {
			cout << ":成交编号" << pTrade->TradeID << " 委托号" << pTrade->OrderRef << " 主场单号" << pTrade->OrderSysID << " " << pTrade->InstrumentID;
			cout << " " << GetDirectionDsp(pTrade->Direction);
			cout << " " << GetOffsetFlagDsp(pTrade->OffsetFlag);
			cout << " P:" << pTrade->Price;
			cout << " " << pTrade->Volume;
		}
		cout << endl;
	}


    //报单回报
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder)
	{
		cout << "::OnRtnOrder" << "\t";
		if (pOrder) {
			cout <<  pOrder->InstrumentID << "|" << pOrder->Direction;
			cout << "|" << pOrder->LimitPrice << "|" << (long)pOrder->VolumeTotal << "|" << pOrder->CombOffsetFlag[0];
			cout << "|" << GetStatusDsp(pOrder->OrderStatus) << "|" << pOrder->OrderSysID << "\t" <<pOrder->StatusMsg<<"\t";
		}
		//QryOrder();
		cout << endl;
	}

	//成交回报
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade)
	{
		cout << "::OnRtnTrade" << "\t";
		if (pTrade) {
			cout << ":成交编号" << pTrade->TradeID << " 委托号" << pTrade->OrderRef << " 主场单号" << pTrade->OrderSysID << " " << pTrade->InstrumentID;
			cout << " " << pTrade->Direction;
			cout << " " << pTrade->OffsetFlag;
			cout << " P:" << pTrade->Price;
			cout << " V:" << pTrade->Volume;
		}
		cout << endl;
	}

	//报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
	{
		cout << ":OnErrRtnOrderInsert" << "\t" << endl;
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "下单成功";
		}
		if (pInputOrder)
			cout << ":" << pInputOrder->InstrumentID << " 价格" << pInputOrder->LimitPrice << " 数量" << pInputOrder->VolumeTotalOriginal << " 方向" << (pInputOrder->Direction == THOST_FTDC_D_Buy ? "买" : "卖");
		cout << endl;
	}
	//报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
	{
		cout << ":OnRspOrderAction" << "\t";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "订单操作成功";
		}
		if (pOrderAction)
			cout << ":" << pOrderAction->OrderSysID << " " << pOrderAction->OrderRef << " " << (pOrderAction->ActionFlag == THOST_FTDC_AF_Delete ? "THOST_FTDC_AF_Delete" : "THOST_FTDC_AF_Modify");
		cout << endl;
	}

	//结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		cout << ":OnRspSettlementInfoConfirm";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "账单确认成功";
		}
		cout << endl;
	}
	
private:
	CThostFtdcTraderApi *m_pUserApi = NULL;
	int RequestID = 0;
	bool m_bMdLogin;
	unsigned long long m_nextOrderRef;
};
