#pragma once
#include "ThostFtdcTraderApi.h"
//#include"Config.h"
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

class CTradeHandler : public CThostFtdcTraderSpi
{
public:
	CTradeHandler();
	~CTradeHandler();
	int Init();
	int ReqTradeLogin();
	//void ReqQryPosition();

private:
	CThostFtdcTraderApi* m_pUserApi = NULL;
	int RequestID = 0;
	bool m_bMdLogin;
	unsigned long long m_nextOrderRef;
};
