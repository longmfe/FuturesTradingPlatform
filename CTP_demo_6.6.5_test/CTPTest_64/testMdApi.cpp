// testTraderApi.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include ".\ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include <Windows.h>
#include <iostream>
#include "TraderSpi.h"
#include "Order.h"
#include "AccountManage.h"
#include "ThostFtdcTraderApi.h"
#include <windows.h>
using namespace std;

// UserApi对象
CThostFtdcMdApi* pUserApi;
CThostFtdcDepthMarketDataField *pDmarket;
// 配置参数
char MFRONT_ADDR[] = "tcp://ctpfz1-front3.citicsf.com:51213";		// 行情前置地址
char TFRONT_ADDR[] = "tcp://ctpfz1-front3.citicsf.com:51205";	   //交易前置地址
TThostFtdcBrokerIDType	BROKER_ID = "66666";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "02001";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "213519";			// 用户密码
char *ppInstrumentID[] = {"m1603", "m1601"};			// 行情订阅列表
int iInstrumentID = 2;									// 行情订阅数量

// 请求编号
int iRequestID = 0;


//UNIT procMarketData()
//{};

DWORD WINAPI MakeOrders(LPVOID lpParameter) //xiadan
{
	AccountManage *AM = new AccountManage;
	CThostFtdcTraderApi *pUserTApi;// = CThostFtdcTraderApi::CreateFtdcTraderApi();
	pUserTApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	  
	pUserTSpi->setPassword(PASSWORD);
	pUserTSpi->setUserName(INVESTOR_ID);
	pUserTSpi->setBrokerID(BROKER_ID);

	CTraderSpi *pUserTSpi = new CTraderSpi(BROKER_ID,pUserTApi);  
	pUserTApi->RegisterSpi((CThostFtdcTraderSpi*)pUserTSpi);
	pUserTApi->RegisterFront(TFRONT_ADDR);
	pUserTApi->SubscribePublicTopic(THOST_TERT_QUICK);				// 注册公有流
	pUserTApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// 注册私有流
		
	pUserTApi->Init();

    Sleep(1000);
	  
	Order *order = new Order( "strategyname", 1, "m1603", 2387, Buy, 0.1, 1, "1515","1512","aaaa", AM) ;//下单

	pUserTSpi->sendOrder(*order);//下单
	return 1;
}


void main(void)
{
	// 初始化UserApi
	//cout<<pDmarket<<endl;

	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	pUserApi->RegisterFront(MFRONT_ADDR);					// connect
	pUserApi->Init();
	cout<<"1"<<endl;
	HANDLE thread = CreateThread(NULL,0,MakeOrders,NULL,0,NULL);
	pUserApi->Join();
	
	//char ch;
    //while(true)
	//{
	//	cin >> ch ;
	//	if(ch='X')
	//	{
	//		break;
	//	}
	//	else
	//	{
	//		Sleep(1000);
	//	}

    //	}
    //	pUserApi->Release();
}