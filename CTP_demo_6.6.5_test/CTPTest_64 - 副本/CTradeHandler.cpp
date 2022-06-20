#pragma once
#include "StdAfx.h"
#include "CTradeHandler.h"
//#include "Config.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>


CTradeHandler::CTradeHandler(void)
{

}

int CTradeHandler :: Init()
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

int CTradeHandler :: ReqTradeLogin()
{
	printf("====ReqTradeLogin====,�û���¼��...%s, %s\n\n", USERID.data(), PASSWORD.data());
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strncpy(reqUserLogin.BrokerID, BROKERID.data(), sizeof(reqUserLogin.BrokerID));
	strncpy(reqUserLogin.UserID, USERID.data(), sizeof(reqUserLogin.UserID));
	strncpy(reqUserLogin.Password, PASSWORD.data(), sizeof(reqUserLogin.Password));
	strncpy(reqUserLogin.TradingDay, "20190524", sizeof(reqUserLogin.TradingDay));
	int re = m_pUserApi->ReqUserLogin(&reqUserLogin, ++RequestID);
	if (re) {
		cout << ": send ReqUserLogin fail: " << re << "," << endl;
	}
	//else cout << ": send ReqUserLogin success: " << re << "," << endl;
	return re;
}

//�����ѯͶ���ֲ߳�
/*
void CTradeHandler :: ReqQryPosition()
{
	CThostFtdcQryInvestorPositionField a = { 0 };
	strcpy_s(a.BrokerID, g_chBrokerID);
	strcpy_s(a.InvestorID, g_chInvestorID);
	string instr;
	instr.clear();
	cin.ignore();
	printf("�������Լ����(��������Ϊ��)��\n");
	getline(cin, instr);
	strcpy_s(a.InstrumentID, instr.c_str());

	string exch;
	exch.clear();
	cin.ignore();
	printf("�����뽻��������(��������Ϊ��)��\n");
	getline(cin, exch);
	strcpy_s(a.ExchangeID, exch.c_str());
	//strcpy_s(a.InstrumentID, "SPD");
	int b = m_pUserApi->ReqQryInvestorPosition(&a, RequestID++);
	printf((b == 0) ? "�����ѯͶ���ֲ߳�......���ͳɹ�\n" : "�����ѯͶ���ֲ߳�......����ʧ�ܣ��������=[%d]\n", b);
}
*/

