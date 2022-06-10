#pragma once
#include"ThostFtdcMdApi.h"
#include"Config.h"
int ordercount = 0;
class CMd :public CThostFtdcMdSpi {
public:
	CMd() {}
	~CMd(){}
	int Init() {
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
	virtual void OnFrontConnected() {
		cout << ":OnFrontConnected_行情前置连接" << endl;
		static const char *version = m_pMdApi->GetApiVersion();
		cout << "------行情当前版本号 ：" << version << " ------" << endl;
		//ReqUserLogin();
	}
	int ReqUserLogin() {
		CThostFtdcReqUserLoginField reqLogin;
		memset(&reqLogin, 0, sizeof(reqLogin));
		strncpy(reqLogin.BrokerID, BROKERID.data(), sizeof(reqLogin.BrokerID));
		//strncpy(reqLogin.UserID, USERID.data(), sizeof(reqLogin.UserID));
		//strncpy(reqLogin.Password, PASSWORD.data(), sizeof(reqLogin.Password));
		int re = m_pMdApi->ReqUserLogin(&reqLogin, ++RequestID);
		if (re) {
			cout << ": send ReqUserLogin fail: " << re << ","  << endl;
		}
		return re;
	}
	int ReqUserLogout() {
		CThostFtdcUserLogoutField reqLogout;
		strncpy(reqLogout.BrokerID, BROKERID.data(), sizeof(reqLogout.BrokerID));
		strncpy(reqLogout.UserID, USERID.data(), sizeof(reqLogout.UserID));
		int re = m_pMdApi->ReqUserLogout(&reqLogout, ++RequestID);
		if (re) {
			cout << ": send ReqUserLogout fail: " << re << ","  << endl;
		}
		return re;
	}
	void Sub(const std::vector<const char*>& instrs) {
		char* *insts = (char**)alloca(sizeof(char*)*(instrs.size()));
		int nCount = 0;
		for (unsigned i = 0; i < instrs.size(); ++i) {
			insts[i] = const_cast<char*>(instrs[i]);
		}
		int re = m_pMdApi->SubscribeMarketData(insts, instrs.size());
		if (re) {
			cout << ": send SubscribeMarketData fail: " << re << ","  << endl;
		}
	}
	void UnSub(const std::vector<const char*>& instrs) {
		char* *insts = (char**)alloca(sizeof(char*)*(instrs.size()));
		int nCount = 0;
		for (unsigned i = 0; i < instrs.size(); ++i) {
			insts[i] = const_cast<char*>(instrs[i]);
		}
		int re = m_pMdApi->UnSubscribeMarketData(insts, instrs.size());
		if (re) {
			cout << ": send UnSubscribeMarketData fail: " << re << ","  << endl;
		}
	}
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspUserLogin_md";
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "行情登录成功";
			m_bMdLogin = true;
			vector <const char*>ve;
			std::string s = INSTRUMENTID;
			ve.push_back((s.c_str()));
			//Sub(ve);
		}
		if (pRspUserLogin)
			cout << ":" << pRspUserLogin->BrokerID << ":" << pRspUserLogin->SystemName << ":" << pRspUserLogin->UserID ;
		cout << endl;
	}
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspUserLogout_md:";
		if (pRspInfo)
			cout << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg ;
		m_bMdLogin = false;
		cout << endl;
	}
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspSubMarketData for " << (pSpecificInstrument ? pSpecificInstrument->InstrumentID : "");
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << "订阅失败" << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "订阅成功";
		}
		cout << endl;
	}
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
		cout << ":OnRspUnSubMarketData for " << (pSpecificInstrument ? pSpecificInstrument->InstrumentID : "");
		if (pRspInfo && pRspInfo->ErrorID) {
			cout << ":" << "取消订阅失败" << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		else {
			cout << ":" << "取消订阅成功";
		}
		cout << endl;
	}
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
		if (pDepthMarketData)
		{
			cout << pDepthMarketData->InstrumentID << "\t";
			cout << pDepthMarketData->UpdateTime << "." << (long)pDepthMarketData->UpdateMillisec << "\t";
			cout << pDepthMarketData->LastPrice << "\t";
			cout << pDepthMarketData->BidPrice1 << "\t";
			cout << (long)pDepthMarketData->BidVolume1 << "\t";
			cout << pDepthMarketData->AskPrice1 << "\t";
			cout << (long)pDepthMarketData->AskVolume1 << "\t";
			cout << (long)pDepthMarketData->Volume;
			cout << endl;
			char str[80];

			sprintf(str, "order SZSE /c %s buy open /v 1 /p %f /q", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
			order(str);
			sprintf(str, "order SZSE /c %s sell close /v 1 /p %f /q", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
			order(str);
			ordercount++;
			if(ordercount==3)
				order("exit");
			
		}
	}
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
	{
		cout << ":OnRspError";
		if (pRspInfo) {
			cout << ":" << pRspInfo->ErrorID << ":" << pRspInfo->ErrorMsg;
		}
		cout << endl;
	}
private:
	CThostFtdcMdApi *m_pMdApi = NULL;
	int RequestID = 0;
	bool m_bMdLogin;
};
