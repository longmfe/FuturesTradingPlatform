#pragma once

#include "StdAfx.h"
#include "CMarketHandler.h"
//#include "Config.h"
#include<vector>
#include <cstring>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
using namespace std;

CMarketHandler::CMarketHandler(void)
{
	
}


CMarketHandler::~CMarketHandler(void)
{
	
}


int CMarketHandler::Init()
{
	m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("");
	m_pMdApi->RegisterSpi(this);
	char market_server_address[128] = { 0 };
	strcpy(market_server_address, MARKET_ADDRESS.data());
	m_pMdApi->RegisterFront(market_server_address);
	if (m_pMdApi) {
		m_pMdApi->Init();
		return 0;
	}
	else {
		return -1;
	}
}
