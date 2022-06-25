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
	hasOpen=false;
	jinChaIndex=0;
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
	inJincha=false;

	a=10;
	b=5;
}

bool TestStrategy::jinCha(){
	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	TickData d=*(ats->pGlobalDm->getMdMana()->getTickData());

	curDiff=ma_1_5->getMAByIndex()-ma_1_10->getMAByIndex();
	preDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);  //�����ڶ�����ֵ������һ��K�ߵľ�ֵ
	if (preDiff<0.0 && curDiff>=0.0)
	{
		//jinChaMin=this->tc.timeToHourmin(d.getTime());
		return true;
	}
	return  false;
}


void TestStrategy::run(){
	


	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");

	ss = *(ats->pGlobalDm->getMdMana()->getTickData());
	

	Bar* bar = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar1" + ss.getSec()));
	/*Bar* bar5 = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar5" + ss.getSec()));*/

	Position* pos = ats->pGlobalDm->getPositionMana()->getPositionBysec(ss.getSec());

	int tick_min=tc.timeToHourmin(ss.getTime());

	int open_pos=pos->getBuyopen();

	if(tick_min>1456 && open_pos>0){   //ÿ������ǰ3����
		Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
			ss.sec, ss.getBidPrice(),Sell, 0, open_pos, ss.getDate(), ss.getTime(),
			ats->espi->exchange,algomanager->getAccountMana());
		esSendOrder(or);
		ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" market is about to close, close long order, Sell, "+ or->getString());

	}
	
	int curBarIndex=bar->getLastBarNum(); //С����bar index
	if(curBarIndex<14){  //����ǰbarС��10+1ʱ�������ܴ���5���ھ��ߺ�10���ھ��߽��
		return;
	}

	
	if(ss.getSec().compare(ats->espi->Contract)!=0)
	{
		return;
	}

	bool isFirst=bar->getIsFirst();
	if(isFirst && ma_1_5->indexList.size()>2 && ma_1_10->indexList.size()>2){  //����tickΪ��ǰbar�ĵ�һ��tickʱ���ж�ǰһ��bar�Ƿ���
		
		double cur5=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2);
		double cur10=ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		curDiff=cur5 - cur10;
		double pre5=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3);
		double pre10=ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		preDiff=pre5 - pre10;  //�����ڶ�����ֵ������һ��K�ߵľ�ֵ

		cout<<"in First tick of this bar: "<<cur5<<", "<<cur10<<", "<<pre5<<", "<<pre10<<endl;

		if (preDiff<0.0 && curDiff>=0.0)
		{
			inJincha=true;
			jinChaIndex=curBarIndex;
		}else if(preDiff>=0.0 && curDiff<0.0){
			inJincha=false;
		}
	}

	int dist_jinCha=curBarIndex-jinChaIndex;
	if(inJincha && !hasOpen && dist_jinCha>3){
		cout<<"inJinCha!!"<<endl;
		double current=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-1)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-1);
		double first=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		double second=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		double third=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-4)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-4);
		if(first<a && second<a && third<a && current>b) {
			
			Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getAskPrice(),Buy, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or);
			cout<<"put order!!"<<endl;
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" inJinCha, Open Long Order, Buy, "+ or->getString());
			hasOpen=true;
		}
	}

	double diff=((BarValue*)bar->getIndex())->getDiff(); 


	//ƽ�ֲ���
	if(isFirst && curBarIndex>0){
		int lastBarIndex=curBarIndex-1;
		double lastDiff=((BarValue*)bar->getIndex(lastBarIndex))->getDiff();
		double tmpHighDiff=((BarValue*)bar->getIndex(tmpHighDiffIndex))->getDiff();
		int dist=lastBarIndex-tmpHighDiffIndex;

		if(dist<4 && lastDiff>=tmpHighDiff){	
			tmpHighDiffIndex=lastBarIndex;
		}else if(dist==3 && lastDiff<tmpHighDiff){
			maxDiffIndex=tmpHighDiffIndex;
			tmpHighDiffIndex=lastBarIndex+1;  //��ʼ��һ��maxDiff�ж�
		}			
	}

	if(pos->getBuyopen()>0){
		double curOpen=((BarValue*)bar->getIndex())->open;
		double maxDiffOpen=((BarValue*)bar->getIndex(maxDiffIndex))->open;
		string maxDiffOpen_str=tc.doubleToString(maxDiffOpen);
		if(curOpen<maxDiffOpen){
			//ƽ��
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
		//	tmpHighDiffIndex=curBarIndex+1;  //��ʼ��һ��maxDiff�ж�
		//}

		//double curClose=((BarValue*)bar->getIndex())->close;
		//double maxDiffClose=((BarValue*)bar->getIndex(maxDiffIndex))->close;
		//if(curClose<maxDiffClose){
		//	//ƽ��
		//	Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
		//		ss.sec, ss.getBidPrice(),Sell, 0, 1, ss.getDate(), ss.getTime(),
		//		ats->espi->exchange,algomanager->getAccountMana());
		//	esSendOrder(or);
		//	ats->pGlobalDm->keepRecord(HisOrder, "close long order, Sell, "+ or->getString());
		//}

	}

	////�������������ж�
	//if(isFirst){
	//	double beforeLastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
	//	double lastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
	//	if(beforeLastDiff<0.0 && lastDiff>=0.0){  //С���ڽ��
	//		smallBarNum=bar->getLastBarNum();
	//		bigBarNum=bar5->getLastBarNum();
	//		smallJinCha=true;
	//	}else if(beforeLastDiff>0.0 && lastDiff<=0.0){
	//		smallJinCha=false;  //С��������
	//	}
	//}

	//if(bar5->getIsFirst()){
	//	if(curBarIndex5<11){
	//		return;
	//	}
	//	double beforeLastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-3)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-3);
	//	double lastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-2)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-2);
	//	if(beforeLastDiff<0.0 && lastDiff>=0.0){  //�����ڷ����˽�棬�ж��������С�����Ƿ��ڽ��״̬
	//		int bigBarNum_cur=bar5->getLastBarNum();
	//		if((bigBarNum_cur-bigBarNum)==1 && smallJinCha){
	//			noClose=true;  //���ÿ�ƽ��flagΪfalse
	//		}
	//	}else if(beforeLastDiff>0.0 && lastDiff<=0.0){  //�����ڷ��������棬���ÿ�ƽ��flagΪtrue
	//		noClose=false;
	//	}
	//}
	
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