#include "StdAfx.h"
#include "MarketApplication.h"
#include "readProperty.h"
#include "UtilFun.h"
MarketApplication* ta= nullptr;
UtilFun uf;
TIMERPROC   FooTimerFunAts2(   HWND,   UINT,   UINT,   DWORD   )
{
    //UtilFun uf;
	int hhmm = atoi(uf.getCHCurDate().substr(8, 4).c_str());
    toolsClass tc;
	string tmp = tc.IntToString(hhmm);            
    if(uf.IsWeekend()){
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":isWeekend");
		}
        return 0;
    }
	
	
	if(hhmm>840&&hhmm<842)// for real
	//if(hhmm>1714&&hhmm<1716)// for test
	{
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":reach reconnect time");
		}
		ta->reConnectForNewDay();
	}else
	{
		if(ta!=NULL)
		{
			ta->writeLog(tmp+":Not reach reconnect time");
		}
	}
               
    //if(!fc.isMDLogon){
	          
    /*if(!ats->bDone1 && hhmm > 845 && hhmm < 1510){
        ats->DoInitWork();
        ats->bDone1 = true;
        ats->bDone2 = false;
    }
                
    if(!ats->bDone2 && hhmm > 1515 && hhmm < 1525){
        ats->DoEndWork();
        ats->bDone2 = true;
        ats->bDone1 = false;
    }*/

	return 0;
}
/*------------------------------------------------------------------------------
Name:
	RegistMdThradFunc
Parm:
	RegMdStruct*
Return:
	UINT
Description
	To regist the market link
------------------------------------------------------------------------------*/
UINT RegistMdThreadFunc(LPVOID lp)
{
	cout<<"RegistMdThreadFunc"<<endl;
	RegMdStruct * m =(RegMdStruct*)lp;
	m->pUserApi->RegisterSpi((CThostFtdcMdSpi*)m->pUserSpi);// ×¢²áÊÂ¼þÀà
	char tmp[100] ={0};
	strcpy(tmp,m->FRONT_ADDRM.c_str());
	m->pUserApi->RegisterFront(tmp);					// connect
	m->pUserApi->Init();
	m->pUserApi->Join(); 
	return 0;
}

MarketApplication::MarketApplication(void):
kc("MarketSessionLogInfo")
{
	readProperty rp;
	username=rp.getUserName();
	password=rp.getPassword();
	brokerID=rp.getbrokerID();
	addrMD=rp.getMDLink();
	isLogon =false;
	rm=NULL;
	pUserMdSpi = NULL;
	ta = this;
	SetTimer( NULL, 10001, 1000*60*1 ,(TIMERPROC)FooTimerFunAts2 ); 
}


MarketApplication::~MarketApplication(void)
{
	if(rm!=NULL)
	{
		delete rm;
		rm =NULL;
	}
	if(pUserMdSpi!=NULL)
	{
		delete pUserMdSpi;
		pUserMdSpi =NULL;
	}
}
void MarketApplication::checkDate(string date1)
{
	if(strcmp(date1.c_str(),currentDate.c_str())==0)
    {
            
    }else
    {
        // new day 
        initial();
        currentDate= date1;
    }
}
void MarketApplication::setFC(CTPClass* fc) 
{
	this->fc = fc;

}
void MarketApplication::initial(){
 
	timeOffset = 0;
	bCheck =false;
 
}
string MarketApplication::getTransTime(string time)
{
	string res = "";
	toolsClass tc;
	int tmp = timeToInt(tc.deleteSubStr(time,' '))+8*60*60;
    if (timeOffset == 0&& !bCheck) {
        if (tmp > timeToInt("91300") && tmp < timeToInt("91500")) {
            timeOffset = timeToInt("91400") - tmp;
           // writeLog.otherLog(TimeUtil.getAllTime()+":timeOffset "+timeOffset);
        } else if (tmp < timeToInt("85940")) {
            timeOffset = timeToInt("85900") - tmp;
           // writeLog.otherLog(TimeUtil.getAllTime()+":timeOffset "+timeOffset);
        } else {
            timeOffset = 0;
        }
        bCheck = true;
    }
    res = IntTotime(tmp + timeOffset);
    return res;
}
string MarketApplication::IntTotime(int IntTime)
{
	string res = "";
    IntTime = IntTime;
    int hh = IntTime / 3600;
    int mm = (IntTime % 3600) / 60;
    int ss = IntTime % 60;
	toolsClass tc;
    if (hh < 10) {
		res = "0" + tc.IntToString(hh);
        if (mm < 10) {
            res += "0" + tc.IntToString(mm);
            if (ss < 10) {
                res += "0" + tc.IntToString(ss);
            } else {
                res += "" + tc.IntToString(ss);
            }
        } else {
            res += "" + tc.IntToString(mm);
            if (ss < 10) {
                res += "0" + tc.IntToString(ss);
            } else {
                res += "" + tc.IntToString(ss);
            }
        }
    } else {
        res = "" + tc.IntToString(hh);
        if (mm < 10) {
            res += "0" + tc.IntToString(mm);
            if (ss < 10) {
                res += "0" + tc.IntToString(ss);
            } else {
                res += "" + tc.IntToString(ss);
            }
        } else {
            res += "" + tc.IntToString(mm);
            if (ss < 10) {
                res += "0" + tc.IntToString(ss);
            } else {
                res += "" + tc.IntToString(ss);
            }
        }
    }
    return res;
}
int MarketApplication::timeToInt(string time)
{

	int res = 0;
    int tmp = 0;
	toolsClass tc;
   
	tmp = tc.stringToInt(time);
    res += (tmp / 10000) * 60 * 60;
    res += (tmp % 10000) / 100 * 60;
    res += tmp % 100;
    return res;
}
string MarketApplication::getTranTime(string tt)
{
	string cc = tt.substr(8,tt.size());
	string ttt = tt.substr(0, 8);
	toolsClass tc;
	string kk =  tc.deleteSubStr(ttt,':');
	return getTransTime(kk)+tc.replace_all_distinct(cc,":",".");

}
void MarketApplication::login()
{
	 
	char bID[11]={0};
	strcpy( bID, brokerID.c_str() ); 
	pUserMdApi =  CThostFtdcMdApi::CreateFtdcMdApi();
	pUserMdSpi = new CMdSpi(bID,pUserMdApi,this);
	pUserMdSpi->setPassword(password);
	pUserMdSpi->setUserName(username);
	rm = new RegMdStruct;
	rm->pUserApi = pUserMdApi;
	rm->pUserSpi = pUserMdSpi;
	rm->FRONT_ADDRM = this->addrMD;
	AfxBeginThread(RegistMdThreadFunc,rm);
	cout<<"AfxBeginThread(RegistMdThreadFunc,rm);"<<endl;

}
void MarketApplication::logout()
{

}
//void MarketApplication::SubcribeMarketDataSingle(string str)
//{
//	this->pUserMdSpi->SubscribeMarketData(str);
//}
void MarketApplication::SubcribeMarketData(string symbolname)
{
	symbolList.insert(pair<string,string>(symbolname,symbolname));
	this->pUserMdSpi->SubscribeMarketData(symbolname);
}
void MarketApplication::reRequestMarketData()
{
	if(!symbolList.empty())
	{
		map<string,string>::iterator it = symbolList.begin();
		for(;it!=symbolList.end();it++)
		{
			SubcribeMarketData(it->second);
		}
	}
}
void MarketApplication::setLogonStatus(bool sta)
{
	this->isLogon =sta;
	int hhmm = atoi(uf.getCHCurDate().substr(8, 4).c_str());
	toolsClass tc;
	string tmptime = tc.IntToString(hhmm);
	if(sta==true)
	{
		this->writeLog(tmptime+":MarketApplication login success");
	
	}else
	{
		this->writeLog(tmptime+":MarketApplication login fail");
	}
	this->fc->setIsMDLogon(sta);
}
bool MarketApplication::getLogonStatus()
{
	return this->isLogon;
}
void MarketApplication::onMarketdata(TickData td)
{
	this->fc->UpdateMarket(td);
}
void MarketApplication::Release()
{
	pUserMdApi->Release();
}

void MarketApplication::writeLog(string parm)
{
	kc.writeLine(parm);
}


void MarketApplication::reConnectForNewDay(void)
{
	int hhmm = atoi(uf.getCHCurDate().substr(8, 4).c_str());
	toolsClass tc;
	string tmptime = tc.IntToString(hhmm);
	this->writeLog(tmptime+":reConnectForNewDay");
	this->Release();
	isLogon =false;
	if(pUserMdSpi!=NULL)
	{
		delete pUserMdSpi;
		pUserMdSpi =NULL;
	}
	if(rm!=NULL)
	{
		delete rm;
		rm = NULL;
	}
	this->login();
}


void MarketApplication::updateContractDetail(string con, double UpperLimitPrice, double LowerLimitPrice, double lastPrice)
{

	this->fc->updateContractDetailByMD(con,UpperLimitPrice,LowerLimitPrice,lastPrice);
}
