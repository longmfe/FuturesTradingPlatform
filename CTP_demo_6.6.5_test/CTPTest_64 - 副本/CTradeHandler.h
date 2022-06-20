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
	///ƽ��
	_OF_Close = '1',
	///ǿƽ
	_OF_ForceClose = '2',
	///ƽ��
	_OF_CloseToday = '3',
	///ƽ��
	_OF_CloseYesterday = '4',
};

enum OrderHedgeFlagType {
	///Ͷ��
	_CHF_Speculation = '1',
	///����
	_CHF_Arbitrage = '2',
	///�ױ�
	_CHF_Hedge = '3',
	///������
	_CHF_MarketMaker = '4',
	///ƥ�����е�ֵ
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
