#include "StdAfx.h"
#include <Windows.h>
#include "AlgoTSEng.h"
#include "GlobalDataManage.h"
#include "TickData.h"
#include "AlgoManager.h"
#include "readContracts.h"
#include "UtilFun.h"
#include "FrameClientDlg.h"
#include "Contract.h"
#include "toolsClass.h" 
#include <iostream>
using namespace std;

AlgoManager* algomanager = nullptr;
extern ContractsManage* g_conM;

/////////告警
DWORD WINAPI PopMsgBox_eng(void* ptr)
{
	string* s = (string*) ptr;
	MessageBoxA(NULL, (LPCSTR)s->c_str(), (LPCSTR)"告警", MB_ICONASTERISK); 
	//delete s;
	return 0;
}

////行情处理，执行策略
UINT procMarketData_e(void* arg)
{ 
	AlgoTSEng* a =(AlgoTSEng*)arg;  
	cout<<"doing1"<<endl;
	while (1)
	{
		try
		{
			if(a->tickl.size() < 1)
			{
				Sleep(1);
				continue;
			}
			else if(a->tickl.size() > 20)
			{
				a->mlock.lock();
				a->tickl.clear();;
				a->mlock.unlock();
				//cout<< "clear now!"<<endl;
				continue;
			}

			a->mlock.lock();

			TickData* tmp = a->tickl.front();
			a->tickl.pop_front();
			a->mlock.unlock();
			//cout<< tmp->sec<< tmp->LastPrice << endl;
			if(NULL == tmp)
			{
				continue;
			} 

			a->process(tmp->getSec(),tmp->getLastPrice(), tmp->getLastVol(), tmp->getAskPrice()
				, tmp->getAskVol(), tmp->getBidPrice(), tmp->getBidVol(), tmp->getOpenInt()
				,tmp->getTradeVol(), tmp->getDate(), tmp->getTime(), 0);
			
			
		}
		catch (CMemoryException* e)
		{
			cout<< "procMarketData_e CMemoryException" << endl;  
		}
		catch (CException* e)
		{
			cout<<"procMarketData_e CException"<<endl;
	    }

	}
	return 0;
}

int curHHMM = 0;
UINT sendDummy(void* arg)
{
	try{
		AlgoTSEng* s = (AlgoTSEng*) arg; 
		while(1)
		{
			Sleep(1000);//every 1 seconds
			if(s->IsPreBarFeeded==false)
			{
				continue;
			}
			UtilFun uf ;
			if(s->IsMorningDummyFeeded==false)
			{
				//855,856,857,858,859,900
				if(uf.getHHMMSS()/100 > curHHMM)
				{
					if(s->sendSingleDummy((uf.getHHMMSS()/100)*100)>0)
					{
						s->DummyCount++;
						if(s->DummyCount >= s->DummyNum )
						{
							s->IsMorningDummyFeeded = true;
						}
					}
				}
			}
			else if(uf.getHHMMSS()/100 > 1515 && uf.getHHMMSS()/100 <=1520 && s->IsMorningDummyFeeded)
			{
				//1501,1502,1503,1504,1505
				if(uf.getHHMMSS()/100 > curHHMM)
				{
					s->sendSingleDummy((uf.getHHMMSS()/100)*100);
				}                                                     
			}

			curHHMM = uf.getHHMMSS()/100;
		}
	}
	catch (CMemoryException* e)
	{
		cout<< "sendDummy CMemoryException" << endl;
	}
	catch (CException* e)
	{
		cout<<"sendDummy CException"<<endl;
	}catch(...)
	{
		
	}
	return 0;
}
UINT feedPreBar(void* arg)
{

	try
	{
		AlgoTSEng* ats1 = (AlgoTSEng*) arg; 
	if(ats1->IsPreBarFeeded)
	{
		return 1;
	}
	int PreBarDayCount = 20; //ats1->qrcs->preLoad();
	char pbdc[5];
	itoa(PreBarDayCount,pbdc,10);
	string strpbdc = pbdc;
	cout<< "preLoaded days: "+ strpbdc << endl;
	ats1->IsPreBarFeeded = true;
	}
	catch (CMemoryException* e)
	{
		cout<< "FeedPreBar CMemoryException" << endl;
		return 0;
	}
	catch (CException* e)
	{
		cout<<"FeedPreBar CException"<<endl;
		return 0;
	}
	return 0;
 
}

void AlgoTSEng::processMarketEvent(TickData* tick)
{
	mlock.lock();
	tickl.push_back(tick); 
	mlock.unlock();
}

AlgoTSEng::AlgoTSEng(string qpistr,AlgoManager* am,bool tradeFlag,bool mdFlag){

	//step . ESPI
	algomanager = am;
	espi = new ESproductInfo();
	espi->string2object(qpistr);

	//step . set those switches.
	this->IsrealMD = mdFlag;//true->CTP MD，false-> HIS ticker
	this->IsrealTrade = tradeFlag;
	this->IsContractsAdded = false;
	this->IsPreBarFeeded = false;
	this->IsMorningDummyFeeded=false;
	this->IsEveDummyFeeded=false;
	this->DummyCount = 0;

	//step . initService
	initService();

	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)procMarketData_e,
		this,
		0,
		NULL);
}

void AlgoTSEng::initService(){
	
	this->pGlobalDm = new GlobalDataManage(true, this);
	fm = new FlowManage();

	//cout<<espi->Name<<endl;
	if(IsrealMD)
	{
		ComputeIndex* ci = new ComputeIndex(pGlobalDm, espi->Contract);
		ComputeIndexUnit* ciu = (ComputeIndexUnit*)ci;
		fm->registerFu((FlowUnits*)ciu);   

		if(!IsrealTrade)
		{
			DealOrdersLocal* dol = new DealOrdersLocal(pGlobalDm);
			DealSendedOrderUnit* dolu = (DealSendedOrderUnit*)dol;
			fm->registerFu((FlowUnits*)dolu);
		}
		//this->pGlobalDm->getIndexMana()->cleanForNewDay();

		//qrcs = new QrcStrategy(this);
		//StrategyUnit* su = (StrategyUnit*)qrcs;
		//fm->registerFu((FlowUnits*)su);  
		esall = new TestStrategy(this);
		StrategyUnit* all = (StrategyUnit*)esall;  
		fm->registerFu((FlowUnits*)all);
/*		esallShort = new TestStrategyShort(this);
		StrategyUnit* all2 = (StrategyUnit*)esallShort;  
		fm->registerFu((FlowUnits*)all2);*/
	

		/*HANDLE thread2 = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)sendDummy,
			(void*)this,
			0,
			NULL);
		HANDLE thread3 = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)feedPreBar,
			(void*)this,
			0,
			NULL);*/
	}
	else
	{
		ComputeIndex* ci = new ComputeIndex(pGlobalDm, espi->Contract);
		ComputeIndexUnit* ciu = (ComputeIndexUnit*)ci;
		fm->registerFu((FlowUnits*)ciu);   

		DealOrdersLocal* dol = new DealOrdersLocal(pGlobalDm);
		DealSendedOrderUnit* dolu = (DealSendedOrderUnit*)dol;
		fm->registerFu((FlowUnits*)dolu);
		
		//esall = new ESStrategyAll(this);
		
		esall=new TestStrategy(this);
		StrategyUnit* all = (StrategyUnit*)esall;  
		fm->registerFu((FlowUnits*)all);  

		/*esallShort=new TestStrategyShort(this);
		StrategyUnit* allShort = (StrategyUnit*)esallShort;  
		fm->registerFu((FlowUnits*)allShort);  */
	}
}

AlgoTSEng::~AlgoTSEng(){
	delete pGlobalDm;
	delete espi;
	delete fm;
}

void AlgoTSEng::process(string& con, double& lastp, double& lastV, double& askp, double& askV,double& bidP, double& bidV, double& openI, double& tradeV, string& date,string& time){
	TickData* td = new TickData();
	td->sec = con;
	td->setAskPrice(askp);
	td->setAskVol(askV);
	td->setBidPrice(bidP);
	td->setBidVol(bidV);
	td->setLastPrice(lastp);
	td->setLastVol(lastV);
	td->setOpenInt(openI);
	td->setTradeVol(tradeV);
	td->setDate(date);
	td->setTime(time);

	this->pGlobalDm->getMdMana()->addTickData(td);

	fm->refresh();
}

void AlgoTSEng::process(string con, double lastp, double lastV, double askp, double askV,double bidP, 
	double bidV, double openI, double tradeV, string date,string time, int xx){
		TickData* td = new TickData();
		td->sec = con;
		td->setAskPrice(askp);
		td->setAskVol(askV);
		td->setBidPrice(bidP);
		td->setBidVol(bidV);
		td->setLastPrice(lastp);
		td->setLastVol(lastV);
		td->setOpenInt(openI);
		td->setTradeVol(tradeV);
		td->setDate(date);
		td->setTime(time);
		
		this->pGlobalDm->getMdMana()->addTickData(td);

		fm->refresh();

}

void AlgoTSEng::CancelMarketData(){
	//psockDm->CancelMd();
}

//for Real Market
void AlgoTSEng::addPosition(ExecReport er){
/*
	this->pGlobalDm->getPositionMana()->addExecReport(er);
	this->pGlobalDm->keepRecord(TradeDetail, er.toString());

	//////////////获取maxshare
	double x = algomanager->X;
	double dms = atof(qpi->MaxSharesFor1X.c_str()) * x;
	int masshare = int(dms + 0.5);
	masshare = masshare > 1?masshare:1;
	/////////////如果持仓数目过多，则报警
	Position* po = this->pGlobalDm->getPositionMana()->getPositionBysec(er.getSec());
	
	if(po->getSellopen() + po->getBuyopen() > masshare)
    {
		//////////告警 
		char ss[100] = {0};
		sprintf(ss, "%d:%d:%d", masshare,po->getSellopen(),po->getBuyopen());
		string str = string(ss);
		string arg = "持仓超过MaxShare设定值!合约:maxshare:卖开:买开，" + er.getSec()
			         + ":" + str  ;
		algomanager->pLog->addlog("maxShare报警+" + arg);
		string* parg = new string(arg);
		HANDLE hPopMsgBox = CreateThread(NULL,0,PopMsgBox_eng,(void*)parg,0,nullptr);
	}
*/
	
}
int AlgoTSEng::addContract(list<string> clist)//for CF to call,add contratclist for me
{
	cout<< "ContractsAdded!"<<endl;
	int result = 0;
	try{
	if(&clist == NULL)
	{
		return 0;
	}
	if(clist.size() == 0)
	{
		return 0;
	}
	else if(clist.size()> 0)
	{
		int initTime = 900;
		if(espi->Name.compare("IF")==0 || espi->Name.compare("if")==0)
		{
			initTime = 915;
		}
		for(list<string>::iterator it = clist.begin(); it != clist.end(); it++ )
		{
			string contractName = *it;
			cout<<contractName<<endl;
			TickData *td = new TickData();
			(this->cmap).insert(pair<string,TickData*>(contractName,td));
			Contract* con = new Contract(contractName,tc.stringToDouble(espi->minMove),tc.stringToInt(espi->pointValue),tc.stringToDouble(espi->commission),tc.stringToDouble(espi->ratioOfmargin));//ID,minMov，pointValue,commision,margin
			//this->pGlobalDm->getContractsMana()->addContract(con);
			g_conM->addContract(con);
			Bar* bar1 = new Bar(1,initTime, contractName);
			this->pGlobalDm->getIndexMana()->registerUnit(bar1);
			if(result != 1)
			{
				result =1;
			}
		}
	}
	if(result == 1)
	{
		this->IsContractsAdded =true;
	}
	}
	catch (CMemoryException* e)
	{
		cout<< "AlgoTSEng::addContract CMemoryException" << endl;
		return 0;
	}
	catch (CException* e)
	{
		cout<<"AlgoTSEng::addContract CException"<<endl;
		return 0;
	}
	return result;
}

void AlgoTSEng::finishTodayWork(){
	this->pGlobalDm->finished();
}


int AlgoTSEng::sendSingleDummy(int curHHMMSS)
{
	if(!IsContractsAdded)
	{
		cout<< "Still no contract added"<<"_"<< this->espi->Strategy<< "_"<<endl;
		return 0;
	}
	if(!IsPreBarFeeded)
	{
		cout<< "Error,PreBarFeeded not finished"<<"_"<< this->espi->Strategy<< "_"<<endl;
		return 0;
	}
	int cnNum = this->cmap.size();
	int cnTmp = 0;
	bool isEnd = false;
	UtilFun uf;
	string date = uf.getCHCurDate().substr(0, 8);
	string time = uf.getCHCurDate().substr(8, 4)+ "00";

	if(cnNum>1)
	{
		for(map<string,TickData*>::iterator itc = this->cmap.begin(); itc != this->cmap.end(); itc++ )
		{
			if(cnNum - cnTmp == 1)//last contract.
			{
				isEnd = true;
			}
			BarValue* bv = new BarValue(date,time,0,0,0,0,0,0,itc->first,itc->first);
			//this->qrcs->dummyFeedin(*bv,0,isEnd);
			cnTmp ++;
		}
	}
	return cnNum;
}

//------------------------------------------------------------------------------------------------------------
//下面三个函数是用来做历史测试的，暂时也不用
void AlgoTSEng::SimulateDayChanged(string yesterday,string today)
{
	//qrcs->initReqtoQRC();
	///qrcs->preFeedin();
	// 暂时写死测试股指。
	initCmap_test("IF",today);
	putContract_cmap_test("IF");
}
int AlgoTSEng::initCmap_test(string pn,string date)
{

	if(date.compare("20130120")<0)
	{
		TickData *td1 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1301",td1));

		TickData *td2 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1302",td2));

		TickData *td3 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1303",td3));

		TickData *td4 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1306",td4));

	}
	else if(date.compare("20130120")>0 && date.compare("20130218")<=0)
	{
		TickData *td1 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1302",td1));

		TickData *td2 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1303",td2));

		TickData *td3 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1306",td3));

		TickData *td4 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1309",td4));

	}
	else if(date.compare("20130218")>0 && date.compare("20130318")<=0)
	{
		TickData *td1 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1303",td1));

		TickData *td2 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1304",td2));

		TickData *td3 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1306",td3));

		TickData *td4 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1309",td4));


	}
	else if(date.compare("20130318")>0 && date.compare("20130330")<=0)
	{
		TickData *td1 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1304",td1));

		TickData *td2 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1305",td2));

		TickData *td3 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1306",td3));

		TickData *td4 = new TickData();
		(this->cmap).insert(pair<string,TickData*>("IF1309",td4));

	}
	return 1;
}
int AlgoTSEng::putContract_cmap_test(string pn)//暂时写死做股指
{
	this->pGlobalDm->getIndexMana()->cleanForNewDay();

	int initTime = 900;
	if(pn.compare("IF")==0 || pn.compare("if")==0)
	{
		initTime = 915;
	}

	for(map<string,TickData*>::iterator itc = cmap.begin(); itc != cmap.end(); itc++ )
	{
		string cn1 = itc ->first;
		Contract* con = new Contract(cn1,0.2,300,0.0001,0.18);
		this->pGlobalDm->getContractsMana()->addContract(con);
		Bar* bar1 = new Bar(1,initTime, cn1);
		this->pGlobalDm->getIndexMana()->registerUnit(bar1);
	}
	return 1;
}