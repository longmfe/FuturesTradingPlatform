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

	//����ʵ��ʹ��
	/*
	if(flagM){
		pm = new PositionManage(nullptr);
		pmR = new PositionManage(nullptr);;//��ѯ�õ��� 
		CreateThread(NULL,0,Loop,(void*)this,0,nullptr); 
	}else
	{
		psockDm = new SocketDama(this);
	}

	pLog = new mLog("aManaLog.txt", true);
	

	if(flagM)
	{
		pLog->addlog("��ʵ����");
	}
	else
	{
		pLog->addlog("��ʷ����");
	}

	if(flagT)
	{
		pLog->addlog("��ʵ����");
	}
	else
	{
		pLog->addlog("ģ�⽻��");
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
	//�жϵڶ����ַ��Ƿ�Ϊ���֣������������ֻ��1���ַ���Ʒ�����ƣ�����������֣���ǰ�����ַ�ΪƷ������
	if (con.at(1) >= 'A')
	{
		res += con.at(0);
		res += con.at(1);
	}
	else
	{
		res = con.at(0);
	}

	//��������Ϲ��򣬷��ؿ�
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
		//#Ʒ�ֲ���
		cout << "Ʒ�ֲ���,������" << pz << endl;
		//pLog->addlog("Ʒ�ֲ���,������" + pz);
		return;
	}
	char tmp[50] = {0};
	sprintf(tmp, "%lf", lastp);
	//pLog->addlog("�������," + con + "," + time + "," + string(tmp));
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

	//���Ҷ�Ӧ��Ҫ�õ���algo
	//Ʒ�֣�����ָ��	map<string, list<AlgoTSEng*>> pzAlgosMap;
	/*
	map<string, list<AlgoTSEng*>>::iterator it;
	it =  pzAlgosMap.find(pz);
	if(it != pzAlgosMap.end())
	{

		//�ַ�
		list<AlgoTSEng*> tmpl = it->second;
		list<AlgoTSEng*>::iterator itl;
		for(itl = tmpl.begin(); itl != tmpl.end(); itl++)
		{
			//pLog->addlog("����ַ�����" + con + "," + time + ",��Լ, " + (*itl)->espi->Contract);
			//(*itl)->processMarketEvent(td); 
		}
	}
	*/
	if(bRealMarket && strcmp(time.c_str(),"150000") >= 0){
		string pinzhong = getPz(con);
		map<string, map<string, double>>::iterator iit;
		//pLog->addlog("TradeVolume��" + pinzhong + "," + con + "," + tc.doubleToString(tradeV));
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
