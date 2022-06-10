// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
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

// UserApi����
CThostFtdcMdApi* pUserApi;
CThostFtdcDepthMarketDataField *pDmarket;
// ���ò���
char MFRONT_ADDR[] = "tcp://ctpfz1-front3.citicsf.com:51213";		// ����ǰ�õ�ַ
char TFRONT_ADDR[] = "tcp://ctpfz1-front3.citicsf.com:51205";	   //����ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "66666";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "02001";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "213519";			// �û�����
char *ppInstrumentID[] = {"m1603", "m1601"};			// ���鶩���б�
int iInstrumentID = 2;									// ���鶩������

// ������
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
	pUserTApi->SubscribePublicTopic(THOST_TERT_QUICK);				// ע�ṫ����
	pUserTApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// ע��˽����
		
	pUserTApi->Init();

    Sleep(1000);
	  
	Order *order = new Order( "strategyname", 1, "m1603", 2387, Buy, 0.1, 1, "1515","1512","aaaa", AM) ;//�µ�

	pUserTSpi->sendOrder(*order);//�µ�
	return 1;
}


void main(void)
{
	// ��ʼ��UserApi
	//cout<<pDmarket<<endl;

	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
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