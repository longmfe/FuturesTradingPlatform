#include "stdafx.h"
#include "TestStrategy.h"
#include <string>
#include <list>
#include "TickData.h"
#include "Order.h"
#include "BarValue.h"
#include "BarDaily.h"
#include "toolsClass.h"
#include "SendToLocal.h"
#include <iostream>
#include <io.h>
#include <direct.h> 
#include "MAUnit.h"
using namespace std;

extern AlgoManager* algomanager;

TestStrategy::TestStrategy(AlgoTSEng* ats){
	this->ats=ats;
	init();
}

TestStrategy::~TestStrategy(void){}

void TestStrategy::init(){
	preDiff=0.0;
	curDiff=0.0;
	hasTrade=false;
	jinChaMin=0;
	maxDiffIndex=0;
	tmpHighDiffIndex=11;

	strategyID = ats->espi->Strategy;
	splitDot=1*this->MinMove;
	isTradingReal = algomanager->IsTradeReal();

	stl = new SendToLocal(ats->pGlobalDm);
	noClose=false;
	bigBarNum=0;
	smallBarNum=0;
	smallJinCha=false;
}

bool TestStrategy::jinCha(){
	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	TickData d=*(ats->pGlobalDm->getMdMana()->getTickData());

	curDiff=ma_1_5->getMAByIndex()-ma_1_10->getMAByIndex();
	preDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);  //倒数第二个均值，即上一个K线的均值
	if (preDiff<0.0 && curDiff>=0.0)
	{
		jinChaMin=this->tc.timeToHourmin(d.getTime());
		return true;
	}
	return  false;
}


void TestStrategy::run(){

	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");

	MAUnit* ma_5_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_5_5");
	MAUnit* ma_5_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_5_10");

	ss = *(ats->pGlobalDm->getMdMana()->getTickData());

	Bar* bar = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar1" + ss.getSec()));
	Bar* bar5 = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar5" + ss.getSec()));

	Position* pos = ats->pGlobalDm->getPositionMana()->getPositionBysec(ss.getSec());

	int tick_min=tc.timeToHourmin(ss.getTime());

	int open_pos=pos->getBuyopen();

	if(tick_min>1456 && open_pos>0){   //每天收盘前3分钟
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			ss.sec, ss.getBidPrice(),Sell, 0, open_pos, ss.getDate(), ss.getTime(),
			ats->espi->exchange,algomanager->getAccountMana());
		esSendOrder(or);
		ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" market is about to close, close long order, Sell, "+ or->getString());

	}

	bool isFirst=bar->getIsFirst();
	bool hasOpen=bar->hasOpenList.back();
	int curBarIndex=bar->getLastBarNum(); //小周期bar index
	int curBarIndex5=bar5->getLastBarNum(); //大周期bar index
	if(curBarIndex<11){  //当当前bar小于10+1时，不可能存在5周期均线和10周期均线金叉
		return;
	}
	
	if(ss.getSec().compare(ats->espi->Contract)!=0)
	{
		return;
	}

	if(isFirst){  //当此tick为当前bar的第一个tick时，判断前一个bar是否开过金叉仓
		bool lastOpen=bar->getLastFlag();
		if(lastOpen && !noClose){
			double lastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
			if(lastDiff<0.0){  //前一个bar最后否定金叉
				//平仓
				Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
					ss.sec, ss.getBidPrice(),Sell, 0, 1, ss.getDate(), ss.getTime(),
					ats->espi->exchange,algomanager->getAccountMana());
				esSendOrder(or);
				ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" deny jinCha, close long order, Sell, "+ or->getString());

				return;
			}
		}
	}

	//MACD* macd1 = new MACD(ss.getSec(), 12, 26, 9, ats->pGlobalDm->getEngineer());
	//double diff=macd1->getYesValue();

	double diff=((BarValue*)bar->getIndex())->getDiff();

	
	bool cross=jinCha();
	if(cross && (diff>0.3 || diff<-0.3) && (!hasOpen)){
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			ss.sec, ss.getAskPrice(),Buy, 0, 1, ss.getDate(), ss.getTime(),
			ats->espi->exchange,algomanager->getAccountMana());
		esSendOrder(or);
		ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" jinCha, Open Long Order, Buy, "+ or->getString());

		bar->hasOpenList.pop_back();
		bar->hasOpenList.push_back(true);  //标记当前bar已经开过仓了
	}


	//平仓策略
	if(isFirst && curBarIndex>0){
		int lastBarIndex=curBarIndex-1;
		double lastDiff=((BarValue*)bar->getIndex(lastBarIndex))->getDiff();
		double tmpHighDiff=((BarValue*)bar->getIndex(tmpHighDiffIndex))->getDiff();
		int dist=lastBarIndex-tmpHighDiffIndex;

		if(dist<4 && lastDiff>=tmpHighDiff){	
			tmpHighDiffIndex=lastBarIndex;
		}else if(dist==3 && lastDiff<tmpHighDiff){
			maxDiffIndex=tmpHighDiffIndex;
			tmpHighDiffIndex=lastBarIndex+1;  //开始下一轮maxDiff判断
		}			
	}

	if(pos->getBuyopen()>0){
		double curOpen=((BarValue*)bar->getIndex())->open;
		double maxDiffOpen=((BarValue*)bar->getIndex(maxDiffIndex))->open;
		string maxDiffOpen_str=tc.doubleToString(maxDiffOpen);
		if(curOpen<maxDiffOpen){
			//平仓
			Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getBidPrice(),Sell, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or);
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" below peak: "+((BarValue*)bar->getIndex(maxDiffIndex))->time+" peak open price: "+maxDiffOpen_str+", close long order, Sell, "+ or->getString());
		}
		
		//double curDiff=((BarValue*)bar->getIndex())->getDiff();
		//double tmpHighDiff=((BarValue*)bar->getIndex(tmpHighDiffIndex))->getDiff();
		//int dist=curBarIndex-tmpHighDiffIndex;

		//if(dist<4 && curDiff>tmpHighDiff){	
		//	tmpHighDiffIndex=curBarIndex;
		//}else if(dist==3 && curDiff<tmpHighDiff){
		//	maxDiffIndex=tmpHighDiffIndex;
		//	tmpHighDiffIndex=curBarIndex+1;  //开始下一轮maxDiff判断
		//}

		//double curClose=((BarValue*)bar->getIndex())->close;
		//double maxDiffClose=((BarValue*)bar->getIndex(maxDiffIndex))->close;
		//if(curClose<maxDiffClose){
		//	//平仓
		//	Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
		//		ss.sec, ss.getBidPrice(),Sell, 0, 1, ss.getDate(), ss.getTime(),
		//		ats->espi->exchange,algomanager->getAccountMana());
		//	esSendOrder(or);
		//	ats->pGlobalDm->keepRecord(HisOrder, "close long order, Sell, "+ or->getString());
		//}

	}

	//继续开仓条件判断
	if(isFirst){
		double beforeLastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		double lastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		if(beforeLastDiff<0.0 && lastDiff>=0.0){  //小周期金叉
			smallBarNum=bar->getLastBarNum();
			bigBarNum=bar5->getLastBarNum();
			smallJinCha=true;
		}else if(beforeLastDiff>0.0 && lastDiff<=0.0){
			smallJinCha=false;  //小周期死叉
		}
	}

	if(bar5->getIsFirst()){
		if(curBarIndex5<11){
			return;
		}
		double beforeLastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-3)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-3);
		double lastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-2)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-2);
		if(beforeLastDiff<0.0 && lastDiff>=0.0){  //大周期发生了金叉，判断其包含的小周期是否处于金叉状态
			int bigBarNum_cur=bar5->getLastBarNum();
			if((bigBarNum_cur-bigBarNum)==1 && smallJinCha){
				noClose=true;  //设置可平仓flag为false
			}
		}else if(beforeLastDiff>0.0 && lastDiff<=0.0){  //大周期发生了死叉，设置可平仓flag为true
			noClose=false;
		}
	}
	
}

void TestStrategy::esSendOrder(Order* order)
{
	if(isTradingReal)
	{
		algomanager->sendorder(order,ats);
	}
	else 
	{
		stl->sendOrder(order);
	}
}