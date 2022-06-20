#include "stdafx.h"
#include "AlgoManager.h" 

//#include "StringUtil.h"
//#include "UtilFun.h" 
//#include "readP.h"
//#include "readIni.h"
CLock lockM;
int AlgoManager::initOrderID = 1000; 

//for Real Market
bool bStart = false; //start CTP or not
bool bRequest = false; //request contracts
bool bInitOK = false;
bool bDomiCon = false;//write Dominant Contract or not

//RequestStore reqStore;
//ContractsManage* g_conM;


//for Real Market


AlgoManager::AlgoManager(bool flagM, bool flagT)
{  
	bRealMarket = flagM;
	bRealTrade = flagT;//bool bRealMarket , bool bRealTrade
	p = 0.50; //alarm when freemoney/equity < p	
	bRequestAllContrct = false;

	//仅供实盘使用
	/*
	if(flagM){
		pm = new PositionManage(nullptr);
		pmR = new PositionManage(nullptr);;//查询得到的 
		CreateThread(NULL,0,Loop,(void*)this,0,nullptr); 
	}else
	{
		psockDm = new SocketDama(this);
	}

	pLog = new mLog("aManaLog.txt", true);
	

	if(flagM)
	{
		pLog->addlog("真实行情");
	}
	else
	{
		pLog->addlog("历史行情");
	}

	if(flagT)
	{
		pLog->addlog("真实交易");
	}
	else
	{
		pLog->addlog("模拟交易");
	}

	init();
	*/
}
AlgoManager::~AlgoManager()
{ 
	/*
	delete pLog;
	if(psockDm !=NULL)
	{
		delete psockDm;
		psockDm = NULL;
	}
	if(accM !=NULL)
	{
		delete  accM ;
		accM =NULL;
	
	*/
}
string AlgoManager::getPz(string con)
{
	string res;
	//判断第二个字符是否为数字，如果是数字则只有1个字符是品种名称；如果不是数字，则前两个字符为品种名称
	if (con.at(1) >= 'A')
	{
		res += con.at(0);
		res += con.at(1);
	}
	else
	{
		res = con.at(0);
	}

	//如果不符合规则，返回空
	if (con.length() > 3 && con.at(2) >= 'A')
	{
		res = "";
	}
	return res;
}

void AlgoManager::OnMarketData(string con, double lastp, double lastV, double askp, double askV,
		         double bidP, double bidV, double openI, double tradeV, string date, string time)
{
	string pz = con;//getPz(con);
	if(pz.length() < 1)
	{
		//#品种不对
		cout << "品种不对,跳过：" << pz << endl;
		//pLog->addlog("品种不对,跳过：" + pz);
		return;
	}
	char tmp[50] = {0};
	sprintf(tmp, "%lf", lastp);
	//pLog->addlog("获得行情," + con + "," + time + "," + string(tmp));
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

	//查找对应需要用到的algo
	//品种，策略指针	map<string, list<AlgoTSEng*>> pzAlgosMap;
	/*
	map<string, list<AlgoTSEng*>>::iterator it;
	it =  pzAlgosMap.find(pz);
	if(it != pzAlgosMap.end())
	{

		//分发
		list<AlgoTSEng*> tmpl = it->second;
		list<AlgoTSEng*>::iterator itl;
		for(itl = tmpl.begin(); itl != tmpl.end(); itl++)
		{
			//pLog->addlog("行情分发至：" + con + "," + time + ",合约, " + (*itl)->espi->Contract);
			//(*itl)->processMarketEvent(td); 
		}
	}
	*/
	if(bRealMarket && strcmp(time.c_str(),"150000") >= 0){
		string pinzhong = getPz(con);
		map<string, map<string, double>>::iterator iit;
		//pLog->addlog("TradeVolume：" + pinzhong + "," + con + "," + tc.doubleToString(tradeV));
		iit = convol.find(pinzhong);
		if(iit == convol.end()){	
			map<string, double> vol;
			vol[con] = tradeV;
			convol[pinzhong] = vol;
		}else{
			map<string, double>::iterator it;
			it = iit->second.find(con);
			if(it == iit->second.end()){
				//map<string, double> vol;
				//vol[con] = tradeV;
				iit->second.insert(map<string, double>::value_type(con, tradeV));
			}else{
				it->second = tradeV;
			}
		}
	}

}
