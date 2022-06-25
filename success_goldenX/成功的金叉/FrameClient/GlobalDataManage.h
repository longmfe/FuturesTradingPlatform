#pragma once
#include <string>
#include "PositionManage.h"
#include "OrdersManage.h"
#include "IndexManage.h"
#include "MarketdataManage.h"
#include "AlgoTSEng.h"
#include "RequestStore.h"
#include "ContractsManage.h"
#include "KeepRecord.h"
using namespace std;

enum FileType{
    HisOrder,ePosition,Account,SendedOrders,TradeDetail,DateEquity,persnal1,Equity,CancelOrders
};

class OrdersManage;
class PositionManage;
class MarketdataManage;
class AlgoTSEng;
class GlobalDataManage{
public:
	GlobalDataManage();
	~GlobalDataManage();
	GlobalDataManage(bool need, AlgoTSEng* eng);
private:
    //��Լ����
    ContractsManage* conM;
    //ָ�����
    IndexManage* indM;
    //�������ݹ���
    MarketdataManage* mdM;
    //��������
    OrdersManage* ordersM;
    //��λ����
    PositionManage* positionM;

    AlgoTSEng* Engineer;   

	
	//for Record
    KeepRecord krHis;
    KeepRecord krPo;
    KeepRecord krAc;
    KeepRecord krSend;
    KeepRecord krTrDe;
    KeepRecord krDateE;
    KeepRecord krPersnal1;
	KeepRecord krequity;
	KeepRecord krcancel;

	void keepRecordAll(string rec);
public:
    AlgoTSEng* getEngineer();    
    void init();

    IndexManage* getIndexMana();
    MarketdataManage* getMdMana();
    OrdersManage* getOrdersMana();
    PositionManage* getPositionMana();
    ContractsManage*  getContractsMana();

	
	//for Record
	void keepRecord(FileType ft, string rec);
	void finishedOneDayWork(bool flag, string datetime, string sec);
	void finished();
};