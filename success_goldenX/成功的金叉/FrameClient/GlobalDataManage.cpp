#include "StdAfx.h"
#include "GlobalDataManage.h"
#include "DoubleIndexValue.h"
#include "Bar.h"
#include "BarDaily.h"
#include <fstream> 

//extern AlgoManager* algomanager;
extern ContractsManage* g_conM;

GlobalDataManage::GlobalDataManage(bool need, AlgoTSEng* eng):
	krHis("HisOrders"),
    krPo("Position"),
    krAc("Account"),
    krSend("SendedOrders"),
    krTrDe("TradeDetail"),
	krDateE(""),
	krPersnal1(""),
	krequity("Equity"),
	krcancel("cancelOrder")
{
	Engineer = eng;
    init();
}
GlobalDataManage::~GlobalDataManage()
{
	if(conM!=NULL)
	{
		delete conM ;
		conM = NULL;
	}
	
    //客户权益管理
	//if(accM !=NULL)
	//{
	//	delete  accM ;
	//	accM =NULL;
	//}
   
    //指标管理
	if(indM!=NULL)
	{
		 delete  indM;
		 indM = NULL;
	}
  
    //行情数据管理
	if(mdM!=NULL)
	{
		 delete  mdM;
		 mdM = NULL;
	}
  
    //报单管理
	if(ordersM!=NULL)
	{
		 delete  ordersM;
		 ordersM = NULL;
	}
  
    //仓位管理
	if(positionM!=NULL)
	{
		 delete   positionM;
		 positionM = NULL;
	}
	/*if(Engineer!=NULL)
	{
		delete Engineer;
		Engineer =NULL;
	}*/
  
}
GlobalDataManage::GlobalDataManage():
	krHis("HisOrders"),
    krPo("Position"),
    krAc("Account"),
    krSend("SendedOrders"),
    krTrDe("TradeDetail"),
	krDateE(""),
	krPersnal1(""),
	krequity("Equity"),
	krcancel("cancelOrder"){
	init();	
}

AlgoTSEng* GlobalDataManage::getEngineer()
{
        return Engineer;
}

void GlobalDataManage::init()
{
    //指标管理
    indM = new IndexManage();
    //行情数据管理
    mdM = new  MarketdataManage(this);
    //报单管理
    ordersM = new OrdersManage(this);
    //仓位管理
    positionM = new PositionManage(this);

	//read contracts from ini
	double minM = atof(Engineer->espi->minMove.c_str());
	int pointV = atoi(Engineer->espi->pointValue.c_str());
	double comm = atof(Engineer->espi->commission.c_str());
	double ratio = atof(Engineer->espi->ratioOfmargin.c_str());
	Contract* con  = new Contract(Engineer->espi->Contract, minM, pointV, comm, ratio);
	g_conM->addContract(con);
}

ContractsManage*  GlobalDataManage::getContractsMana()
{
        return conM;
}
IndexManage* GlobalDataManage::getIndexMana()
{
        return indM;
}
MarketdataManage*  GlobalDataManage::getMdMana()
{
        return mdM;
}
OrdersManage*  GlobalDataManage::getOrdersMana()
{
        return ordersM;
}
PositionManage*  GlobalDataManage::getPositionMana()
{
        return positionM;
}


void  GlobalDataManage::keepRecord(FileType ft, string rec)
{

        if(ft == FileType::Account){
                krAc.appendData(rec);
        }
        else if(ft == FileType::HisOrder){
                krHis.appendData(rec);
        }
        else if(ft == FileType::ePosition){
                krPo.appendData(rec);
        }
        else if(ft == FileType::SendedOrders){
                krSend.appendData(rec);
        }
        else if(ft == FileType::TradeDetail){
                krTrDe.appendData(rec);
        }
		else if (ft == FileType::Equity)
		{
			krequity.appendData(rec);
		}
		else if(ft == FileType::CancelOrders)
		{
			krcancel.appendData(rec);
		}
}
void  GlobalDataManage::keepRecordAll(string rec)
{		
        krAc.appendData(rec);
        krHis.appendData(rec);
        krPo.appendData(rec);
        krSend.appendData(rec);
        krTrDe.appendData(rec);		
		krequity.appendData(rec);
		krequity.appendData(rec);
}

void  GlobalDataManage::finishedOneDayWork(bool flag, string datetime, string sec)
{
	BarDaily* bd = (BarDaily*)(getIndexMana()->getIndexUnit("BarDaily" + sec));
	bd->writeBDtoFile(sec);
	Bar* bar1 = (Bar*)(getIndexMana()->getIndexUnit("Bar1" + sec));
	bar1->writeBartoFile(sec);

/*        krPo.appendData(positionM->getString());
        //krAc.appendData(accM->toString());
        krSend.appendData(ordersM->getString());
        keepRecordAll("Wait for the new day:" + datetime);

		//add by ccf  for test
		ofstream of;s
		of.open("./outIndex.txt",ios::trunc); 
		//CloseIndexUnit* close1 = (CloseIndexUnit*) pGlobalDm->getIndexMana()->getIndexUnit("Close1IF888");
		//int len = close1->getSize();
		//for(int i = 0 ; i < len; i++){
		//	of << (dynamic_cast<DoubleIndexValue * >( (close1)->getIndex(i) )).getValue() << endl;
		//}
		//end

        if(flag){
            if(positionM->cleanForNewDay()){
                cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Po"<<endl;
            }
            //AlgoTSEng.GlobalDm.getAccountMana().cleanForNewDay();
            indM->cleanForNewDay();
        }
        Engineer->finishTodayWork();*/
}

void  GlobalDataManage::finished()
{
    krHis.Finish();
    krPo.Finish();
    krAc.Finish();
    krSend.Finish();
    krTrDe.Finish();
    krDateE.Finish();
    krPersnal1.Finish();
	krequity.Finish();
	krcancel.Finish();
}

