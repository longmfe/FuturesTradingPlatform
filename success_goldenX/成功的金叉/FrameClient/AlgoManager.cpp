#include "stdafx.h"
#include "AlgoManager.h" 

#include "StringUtil.h"
#include "UtilFun.h" 
#include "readP.h"
#include "readIni.h"
CLock lockM;
int AlgoManager::initOrderID = 1000; 

//for Real Market
bool bStart = false; //start CTP or not
bool bRequest = false; //request contracts
bool bInitOK = false;
bool bDomiCon = false;//write Dominant Contract or not

RequestStore reqStore;
ContractsManage* g_conM;


//for Real Market
DWORD WINAPI Loop(void* ptr)
{
	AlgoManager* s = (AlgoManager*) ptr; 
	while(1)
	{
		Sleep(1000*20);
		s->pLog->addlog("����ѭ��");
		UtilFun uf ;
		if(!bStart && uf.getHHMMSS() > 85000)
		{
			s->pLog->addlog("��ʼ��CTPCLASS");
			bStart = true;
			s->ctp = new CTPClass(s); 
			s->ctp->runCTP();
		}

		if(bStart && !bRequest && bInitOK)
		{
			if(s->ctp->getIsLogonSuccess())
			{
				s->pLog->addlog("�ж�CTP��½�ɹ����������к�Լ");
				cout<<"�ж�CTP��½�ɹ����������к�Լ"<<endl;
				bRequest = true;
				s->reqAllCons();
				continue;
			}
			if(s->ctp->getIsMDLogon() && !s->ctp->getIsTradeLogon() &&s->IsMDReal() && !s->IsTradeReal()){
				s->pLog->addlog("�ж�ֻ������CTP��½�ɹ������������Լ");
				cout<<"�ж�ֻ������CTP��½�ɹ������������Լ"<<endl;
				bRequest = true;
				map<string, list<AlgoTSEng*>>::iterator iit;
				for(iit = s->pzAlgosMap.begin();iit != s->pzAlgosMap.end();iit++){
					s->addContracts(iit->first, 0);
				}
				continue;
			}
		}

		if(bStart && bRequest)
		{
			s->pLog->addlog("Ϊ����֤�������ʽ�ͳֲ�");
			cout<<"Ϊ����֤�������ʽ�ͳֲ�"<<endl;
			s->reqAccount();
			s->reqPositions();
		}

		//judge Dominant Contracts using ES server
		/*if(s->IsMDReal() && uf.getHHMMSS()>145000 && uf.getHHMMSS()<145500){	
			if(!s->bRequestAllContrct){
				s->bRequestAllContrct = true;			
				s->reqAllCons();
			}
		}*/		

		if(uf.getHHMMSS() >= 150100 && s->IsMDReal() && !bDomiCon){
			s->pLog->addlog("д��������");
			s->finishTheWork(false, uf.getCurDate() + "---" + s->tc.IntToString(uf.getHHMMSS()));

			//s->pLog->addlog("�ж�������Լ");
			//s->judgeDominantContract();
			bDomiCon = true;
			//s->rc.writeSingleCfg(s->args);
		}
	}
	return 0;
}
DWORD WINAPI PopMsgBox(void* ptr)
{
	string* s = (string*) ptr;
	MessageBoxA(NULL, (LPCSTR)s->c_str(), (LPCSTR)"�澯", MB_ICONASTERISK); 
	//delete s;
	return 0;
}

AlgoManager::AlgoManager(bool flagM, bool flagT)
{  
	bRealMarket = flagM;
	bRealTrade = flagT;//bool bRealMarket , bool bRealTrade
	p = 0.50; //alarm when freemoney/equity < p	
	bRequestAllContrct = false;

	//����ʵ��ʹ��
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
}
AlgoManager::~AlgoManager()
{ 
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
	}
}

void AlgoManager::init()
{	 
	 list<string> args = rc.getArgs(bRealMarket);

	 g_conM = new ContractsManage(true);
	 accM = new  AccountManage("Wo",1000000); 

	 list<string>::iterator it;
	 StringUtil su;
	 for(it = args.begin(); it != args.end(); it++)
	 {
		 list<string> tmpl = su.stringAnalysis(*it,",");
		 //��һ��Ʒ��,�ڶ�����Լ
		 string pz = tmpl.front();
		 tmpl.pop_front();
		 string id = tmpl.front();		 		 
		 g_cons.push_back(id);
		 		 
	     //Ʒ�֣�����ָ��	map<string, AlgoTSEng*> idAlgoMap;
		 AlgoTSEng* al = new AlgoTSEng(*it, this, bRealTrade, bRealMarket);
		 idAlgoMap[pz] = al;

		 //��ԼID������ָ��	map<string, list<AlgoTSEng*>> pzAlgosMap;
		 map<string, list<AlgoTSEng*>>::iterator iit;
		 iit = pzAlgosMap.find(id);
		 if(iit == pzAlgosMap.end())
		 {
			 pLog->addlog("��ʼ������--����Ʒ��," + id + ",Ʒ��, " + pz);
			 cout<<"��ʼ������--����Ʒ��," + id + ",Ʒ��, " + pz<<endl;
			 list<AlgoTSEng*> ll;
			 ll.push_back(al);
			 pzAlgosMap[id] = ll;
		 }
		 else
		 {
			 pLog->addlog("��ʼ������--���Ʒ��," + id + ",Ʒ��, " + pz);
			  cout<<"��ʼ������--���Ʒ��," + id + ",Ʒ��, " + pz<<endl;
			 iit->second.push_back(al);
		 }
	 }	 
	 
	 if(!bRealMarket){	 

		 list<string> dates;
		 dates.push_back("20150310-20150310");
		 reqStore.addMdRequest(1, g_cons, dates, 0, 0); 
		 readIni ri;
		 psockDm->setServerIP(ri.getServerIp());
		 psockDm->setPort(ri.getServerPort());
		 psockDm->setClientID(ri.getClientId());
		 psockDm->Clientservice();
	 }	 

	 pLog->addlog("��ʼ�����------------------------------------------");
	 bInitOK = true;
}

int AlgoManager::getLastOrderID()
{
	int id;	
	lockM.lock();
	id = initOrderID++; 
	lockM.unlock();
	return id;
}
  
//for history backtest
AccountManage* AlgoManager::getAccountMana()
{
     return accM;
}

void AlgoManager::finishTheWork(bool flag, string datetime){
	map<string, list<AlgoTSEng*>>::iterator it;
	for(it = pzAlgosMap.begin(); it != pzAlgosMap.end(); it++)
	{
		list<AlgoTSEng*> tmpl = it->second;
		list<AlgoTSEng*>::iterator itl;
		for(itl = it->second.begin() ; itl !=  it->second.end(); itl ++)
		{
			(*itl)->pGlobalDm->finishedOneDayWork(flag, datetime,it->first);
		}
	}
}

//judeg Dominant Contract using ES server
/*void AlgoManager::judgeDominantContract(){
	list<string>::iterator it;
	StringUtil su;
	map<string, map<string, double>>::iterator iit;			
	map<string, double>::iterator itvol;

	for(iit = convol.begin();iit!=convol.end();iit++){
		for(itvol = iit->second.begin(); itvol !=iit->second.end(); itvol++){
			for(it = args.begin(); it != args.end(); it++){
				 list<string> tmpl = su.stringAnalysis(*it,",");
				 //��һ��Ʒ��,�ڶ�����Լ,�������ɽ���
				 string pz = tmpl.front();
				 tmpl.pop_front();
				 string con = tmpl.front();		 		 
				 tmpl.pop_front();
				 string volstr = tmpl.front();
				 double vol = tc.stringToDouble(volstr);

				 if(itvol->second > vol * 1.1 && strcmp(itvol->first.c_str(), con.c_str()) != 0){
					 string out_str = *it;    
					 int pos = (*it).find(con);    
					 if (pos != NULL){
						 out_str =(*it).replace(pos, con.length() + volstr.length() + 1, 
							 itvol->first + "," + tc.doubleToString(itvol->second));    
					 }
				 }
			}
		}
	}
}*/

void AlgoManager::addContracts(string con, int flag)
{
	if(con.length() > 7)
	{
		return;
	}
	pLog->addlog("����º�Լ--" + con);
	//����� 	map<string, list<string>> pzConsMap;
	string pz = getPz(con);
	pLog->addlog("Ʒ���ж�Ϊ--" + pz);
	if(pz.length() > 0){ 
		 //��ѯpzAlgosMap����֤�Ƿ���Ҫ��Ʒ�ֺ�Լ
		map<string, list<AlgoTSEng*>>::iterator it1 = pzAlgosMap.find(con);
		if(it1 == pzAlgosMap.end())
		{
			//û���ҵ���������Ҫ���Ĵ�Ʒ����غ�Լ
			pLog->addlog("����Ҫ���Ĵ�Ʒ����غ�Լ--" + pz);
			cout<<"����Ҫ���Ĵ�Ʒ����غ�Լ--" + pz<<endl;
		}
		else
		{
			pzConsMap[pz].push_back(con);
			pLog->addlog("��������--" + con);
			cout<<"��������--" + con<<endl;
			ctp->requestMD(con);
		}	     
	}
	else
	{
		pLog->addlog("Ʒ�ֽ綨������--" + pz);
		//return;
	}

	if(bRealMarket && bRequestAllContrct){
		map<string, list<string>>::iterator it1 = pzConsMap.find(pz);
		if(it1 != pzConsMap.end())
		{
			pLog->addlog("�����������Ʒ������--" + con);
			ctp->requestMD(con);
		}     
	}
}

void AlgoManager::addPosition(ExecReport er)
{
	//���Ҳ��ַ�����Ӧalgo
	//����ID������ָ��	map<string, AlgoTSEng*> idAlgoMap;
	map<string, AlgoTSEng*>::iterator it;
	it =  idAlgoMap.find(er.getStrategyID());
	char tmp[50] = {0};
	sprintf(tmp, "%d,%d,%lf", er.getOrderID(), er.getTradeVol(), er.getTradePrice());
	pLog->addlog("��óɽ��ر�--StraID," + er.toString());
	if(it != idAlgoMap.end())
	{
		pLog->addlog("ת���ɽ��ر�--");
		it->second->addPosition(er);
	}

	//��ӱ���
	pm->addExecReport(er);
}
string AlgoManager::getPz(string con)
{
	string res;
	//�жϵڶ����ַ��Ƿ�Ϊ���֣������������ֻ��1���ַ���Ʒ�����ƣ�����������֣���ǰ�����ַ�ΪƷ������
	if(con.at(1) >= 'A')
	{
		res += con.at(0);
		res += con.at(1);
	}
	else
	{
		res = con.at(0);
	}

	//��������Ϲ��򣬷��ؿ�
	if(con.length() > 3 && con.at(2) >= 'A')
	{
		res = "";
	}
	return res;
}
////####δ�깤�����������ordermanage
bool AlgoManager::sendorder(Order* order, AlgoTSEng* ae){
	map<string, AlgoTSEng*>::iterator it =  idAlgoMap.find(order->getStrategyID());

	char tmp[50] = {0};
	sprintf(tmp, "%d", order->getOrderID());
	pLog->addlog("ת������--StraID," + ae->espi->Strategy + ",orid, " + string(tmp));

	///////////////////////////////////////////////////
	//////�ж��Ƿ�Ϊ�м۵���Ȼ����QRC�ĵ�����
	//��ȡ��ߡ���͡����¼ۣ���С�ƶ���
	if(order->getSendPrice() < 1)
	{
		contractDetail * cd = ctp->getContractDetail(order->getSec());

		if(cd == NULL)
		{
			pLog->addlog("�Ҳ�����Ӧ��Լ, " + order->getSec()); 
		}
		else
		{
			double uplimit = cd->UpperLimitPrice;
			double downlimit = cd->LowerLimitPrice;
			double lastp = cd->lastPrice;
			double minmove = cd->PriceTick;

			if(order->getType() == Stype::Buy || order->getType() == Stype::Buytocover)
			{
				double price = lastp*1.02 < uplimit? lastp*1.02:uplimit;
				price = ((int)((price + minmove*0.1)/minmove))*minmove; 
				order->SendPrice = price;

				char ttmp[50] = {0};
				sprintf(ttmp, "%lf", price);
				pLog->addlog("�м۵�,orid, " + string(tmp) + ",�����۸�," + string(ttmp)); 
			}

			if(order->getType() == Stype::Sell || order->getType() == Stype::Sellshort)
			{
				double price = lastp*0.98 > downlimit? lastp*0.98:uplimit;
				price = ((int)((price + minmove*0.1)/minmove))*minmove; 
				order->SendPrice = price;

				char ttmp[50] = {0};
				sprintf(ttmp, "%lf", price);
				pLog->addlog("�м۵�,orid, " + string(tmp) + ",�����۸�," + string(ttmp)); 
			}
		}
	}
	///////////////////////////////////////////////////

	if(it != idAlgoMap.end())
	{
		pLog->addlog("�������Ӧ��Algo��OrderMana");
		ae->pGlobalDm->getOrdersMana()->SendedOrder(order);
	}
	else
	{
        //#### alert
		pLog->addlog("�������Ӧ��Algo��OrderMana,��Ϊδ�ҵ���Ӧ��ALGO�������algomana��ӦidAlgoMap��StraID��" + ae->espi->Strategy );
		idAlgoMap[order->getStrategyID()] = ae;
		ae->pGlobalDm->getOrdersMana()->SendedOrder(order);

	}
	return ctp->sendorder(order);
	
}
bool AlgoManager::cancelOrder(Order* order, AlgoTSEng* ae){ 
	char tmp[50] = {0};
	sprintf(tmp, "%d", order->getOrderID());
	pLog->addlog("ת������--StraID," + ae->espi->Strategy + ",orid, " + string(tmp));
	return ctp->cancelOrder(order);
}
void AlgoManager::OnRspAccount(double equity, double freemoney)//Ȩ�棬�����ʽ�
{
	char s[50] = {0};
	sprintf(s, "%lf,%lf", equity, freemoney);
	string ss = string(s);
	pLog->addlog("�ʽ���֤,����," + ss);
	if(equity < 1)
	{
		//����
		UtilFun uf;
		char ss[100];
		sprintf(ss, "%s", uf.getCHCurDate().c_str());

		char strs[12] = {0};
		sprintf(strs, "%lf", equity);
		string str = string(strs);

		string arg = string(ss) + "---����Ȩ��̫��-" + str;
		pLog->addlog("����+" + arg);
		string* parg = new string(arg);
		HANDLE hPopMsgBox = CreateThread(NULL,0,PopMsgBox,(void*)parg,0,nullptr);
	}

	if(freemoney / equity < p)
	{
		//����
		UtilFun uf;
		char ss[100] = {0};;
		sprintf(ss, "%s", uf.getCHCurDate().c_str());

		char strs[50] = {0};
		sprintf(strs, "%lf", equity); 

		char strs1[50] = {0}; 
		sprintf(strs1, "%lf", freemoney); 

		char strs2[50] = {0};
		sprintf(strs2, "%lf", p); 

		string arg = string(ss) + "---�����ʽ���٣�Ȩ�棺�����ʽ�Ԥ����ֵ--" + string(strs) + ":"
			           + string(strs1) + ":"  + string(strs2);
		pLog->addlog("����+" + arg);
		string* parg = new string(arg);
		HANDLE hPopMsgBox = CreateThread(NULL,0,PopMsgBox,(void*)parg,0,nullptr);
	}
}

void AlgoManager::OnRspPosition(string con, int dir, int lots, double price, int flag)//con, direction, lots, islast
{
	if(flag > 0)
	{
		//���һ�����
		ExecReport er;
		er.setSec(con);
		er.setType((Stype)dir);
		er.setTradeVol(lots);
		er.setTradePrice(price);
		pmR->addExecReport(er);

		pLog->addlog("�������ֲֻر�+" + er.toString());
		pLog->addlog("�ֲֻر�������������֤�׶�+" + er.toString());
		//�Ƚ�pm��pmR,��pmRΪ׼������pmR������pm������һ�����׳�һ����,��Ϊ�������������ȷ��
		map<string, Position*>::iterator it;
		for(it = pmR->orderMap.begin(); it != pmR->orderMap.end(); it++)
		{
			string sec = it->second->getSec();
			Position* globalP = pm->getPositionBysec(sec);
			if(it->second->getBuyopen() != globalP->getBuyopen())
			{
				char strs[12] = {0};
				_itoa(it->second->getBuyopen(), strs, 10);
				string str = string(strs);

				char strs1[12] = {0};
				_itoa(globalP->getBuyopen(), strs1, 10);
				string str1 = string(strs1);

				string arg = "�ֲֲ��ԣ���Լ:CTP������:�����¼����--" + sec + ":" + str + ":"+ str1 ;
				pLog->addlog("����+" + arg);
				string* parg = new string(arg);
				HANDLE hPopMsgBox = CreateThread(NULL,0,PopMsgBox,(void*)parg,0,nullptr);
			}

			if(it->second->getSellopen() != globalP->getSellopen())
			{
				char strs[12] = {0};
				_itoa(it->second->getSellopen(), strs, 10);
				string str = string(strs);

				char strs1[12] = {0};
				_itoa(globalP->getSellopen(), strs1, 10);
				string str1 = string(strs1);

				string arg = "�ֲֲ��ԣ���Լ:CTP��������:�����¼����--" + sec + ":" + str + ":"+ str1 ;
				pLog->addlog("����+" + arg);
				string* parg = new string(arg);
				HANDLE hPopMsgBox = CreateThread(NULL,0,PopMsgBox,(void*)parg,0,nullptr);
			}			
		}
		pLog->addlog("�ֲ���֤���");
	}
	else
	{
		ExecReport er;
		er.setSec(con);
		er.setType((Stype)dir);
		er.setTradeVol(lots);
		er.setTradePrice(price);
		pmR->addExecReport(er);

		pLog->addlog("�������ֲֻر�+" + er.toString());
	}
}
void AlgoManager::OrderUpdate(int orderid , string strId, int status)//orderid, stringid, orderstatus
{
	//���Ҳ��ַ�����Ӧalgo
	//����ID������ָ��	map<string, AlgoTSEng*> idAlgoMap;
	map<string, AlgoTSEng*>::iterator it;

	char tmp[50] = {0};
	sprintf(tmp, "%d,%d", orderid, status);
	pLog->addlog("���״̬�ر�--StraID," + strId + "," + string(tmp));

	it =  idAlgoMap.find(strId);
	if(it != idAlgoMap.end())
	{
		//###it->second->) ���Ҷ�Ӧ��order������֮
		Order* porder = it->second->pGlobalDm->getOrdersMana()->getOrderByID(orderid);
		
		if(status==Order::Default||status==Order::Reject)
		{
			if(status==Order::Reject)
			{
				if(porder->status==Order::Filled||porder->status==Order::PartialFill)
				{
						
				}else
				{
					porder->status=status;
				}

				pLog->addlog("�Ƴ�δ�ɽ�����--StraID," + strId + "," + string(tmp));
				it->second->pGlobalDm->getOrdersMana()->removeOrderFromUnTraderOrderByID(porder->orderID);
			}
		}else
		{
			pLog->addlog("���ñ���״̬--StraID," + strId + "," + string(tmp));
			porder->setStatus(status);
			if(status==Order::Filled||status==Order::Cancelled)
			{
				pLog->addlog("�Ƴ�δ�ɽ�����--StraID," + strId + "," + string(tmp));
				it->second->pGlobalDm->getOrdersMana()->removeOrderFromUnTraderOrderByID(porder->orderID);
			}
		}	
		
	}
}
//CALL CTP
void AlgoManager::reqPositions()
{
	
	pLog->addlog("����ֲ֣�����֤");
 
	pmR->cleanForNewDay();
	ctp->requestPosition();
}
void AlgoManager::reqAccount()
{
	pLog->addlog("�����ʽ𣬴���֤");
	ctp->requestAccount();
}
void AlgoManager::reqAllCons()
{
	pLog->addlog("�������к�Լ");
	ctp->requestInstrument();
} 


void AlgoManager::OnMarketData(string con, double lastp, double lastV, double askp, double askV,
		         double bidP, double bidV, double openI, double tradeV, string date, string time)
{
	string pz = con;//getPz(con);
	if(pz.length() < 1)
	{
		//#Ʒ�ֲ���
		pLog->addlog("Ʒ�ֲ���,������" + pz);
		return;
	}
	char tmp[50] = {0};
	sprintf(tmp, "%lf", lastp);
	pLog->addlog("�������," + con + "," + time + "," + string(tmp));
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
	map<string, list<AlgoTSEng*>>::iterator it;
	it =  pzAlgosMap.find(pz);
	if(it != pzAlgosMap.end())
	{

		//�ַ�
		list<AlgoTSEng*> tmpl = it->second;
		list<AlgoTSEng*>::iterator itl;
		for(itl = tmpl.begin(); itl != tmpl.end(); itl++)
		{
			pLog->addlog("����ַ�����" + con + "," + time + ",��Լ, " + (*itl)->espi->Contract);
			(*itl)->processMarketEvent(td); 
		}
	}

	if(bRealMarket && strcmp(time.c_str(),"150000") >= 0){
		string pinzhong = getPz(con);
		map<string, map<string, double>>::iterator iit;
		pLog->addlog("TradeVolume��" + pinzhong + "," + con + "," + tc.doubleToString(tradeV));
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

bool AlgoManager::IsMDReal()
{
	return this->bRealMarket;
}
void AlgoManager::setMDReal(bool real)
{
	this->bRealMarket = real;
}
bool AlgoManager::IsTradeReal()
{
	return this->bRealTrade;
}
void AlgoManager::setRealTrade(bool real)
{
	this->bRealTrade=real;
}