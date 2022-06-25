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
	this->xiancha.insert(pair<int,double> (999999,999999));
	this->jincha.insert(pair<int,double> (999999,999999));
	this->nixiancha.insert(pair<int,double> (999999,999999));
	this->nijincha.insert(pair<int,double> (999999,999999));
	Bopen = false;
	Sopen = false;

	a=10;
	b=5;
}

bool TestStrategy::jinCha(){

    MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	MAUnit* ma_1_3=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_3");
	MAUnit* ma_1_1=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_1");
	if (ma_1_1->indexList.size() < 11)
	{
		return false;
	}
	int i = 1;
	double sum = 0;
	for( i = 1; i != 11; i++)
	{
		sum = sum + abs(ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i) - ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i - 1));

	};

	double var1 = ma_1_5->getMAByIndex();
	double er = (ma_1_1->getMAByIndex() - ma_1_10->getMAByIndex())/sum;
	double sc = er * 0.6022+0.0645;
	double s2 = var1 + sc*sc*(ma_1_1->getMAByIndex() - var1);
	double s1 = ma_1_3->getMAByIndex();
	double s12 = s1-s2;
	map<int,double> ::iterator ptmpjinchalist;
	ptmpjinchalist = this->jincha.begin();
	int j=1;
	/*for(ptmpjinchalist= jincha.begin();ptmpjinchalist != jincha.end();ptmpjinchalist ++ )
	{
		cout<<"第"<<j<<"个元素"<<endl;
		cout<<"第一个："<<ptmpjinchalist->first<<";第二个:"<<ptmpjinchalist->second<<endl;
		j++;
	}*///测试问题，999999初始化中被放入了最后一个格子，之后都是正确的插入

     if(jincha.size()>10)
	{

		jincha.erase(ptmpjinchalist);
		jincha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	}
	else
	{
		jincha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	};
	 if(jincha.size()>3)
	 {
	ptmpjinchalist = jincha.end();
	ptmpjinchalist --;
	ptmpjinchalist --;
//	cout<<"last-first"<<ptmpjinchalist->first<<endl;
	double tmpfinal = ptmpjinchalist->second;
	ptmpjinchalist --;

	double tmpbeforefinal = ptmpjinchalist->second;
//	cout<<"last-first"<<ptmpjinchalist->first<<endl;
	cout<<"lastlast"<<tmpbeforefinal<<"last"<<tmpfinal<<endl;
//	cout<<"size of jinchalist"<<jincha.size()<<endl;
	if(tmpbeforefinal<=0 && tmpfinal>0)
	{
	return true;
	}
	else 
	{

	return false;

	};
	 } 
	 else
		 {return false;
	 };
	

};

bool TestStrategy::nijinCha(){

    MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	MAUnit* ma_1_3=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_3");
	MAUnit* ma_1_1=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_1");
	if (ma_1_1->indexList.size() < 11)
	{
		return false;
	}
	int i = 1;
	double sum = 0;
	for( i = 1; i != 11; i++)
	{
		sum = sum + abs(ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i) - ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i - 1));

	};

	double var1 = ma_1_5->getMAByIndex();
	double er = (ma_1_1->getMAByIndex() - ma_1_10->getMAByIndex())/sum;
	double sc = er * 0.6022+0.0645;
	double s2 = var1 + sc*sc*(ma_1_1->getMAByIndex() - var1);
	double s1 = ma_1_3->getMAByIndex();
	double s12 = s1-s2;
	map<int,double> ::iterator ptmpnijinchalist;
	ptmpnijinchalist = this->nijincha.begin();
	int j=1;
	/*for(ptmpjinchalist= jincha.begin();ptmpjinchalist != jincha.end();ptmpjinchalist ++ )
	{
		cout<<"第"<<j<<"个元素"<<endl;
		cout<<"第一个："<<ptmpjinchalist->first<<";第二个:"<<ptmpjinchalist->second<<endl;
		j++;
	}*///测试问题，999999初始化中被放入了最后一个格子，之后都是正确的插入

     if(nijincha.size()>10)
	{

		nijincha.erase(ptmpnijinchalist);
		nijincha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	}
	else
	{
		nijincha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	};
	 if(nijincha.size()>3)
	 {
	ptmpnijinchalist = nijincha.end();
	ptmpnijinchalist --;
	ptmpnijinchalist --;
//	cout<<"last-first"<<ptmpnijinchalist->first<<endl;
	double tmpfinal = ptmpnijinchalist->second;
	ptmpnijinchalist --;

	double tmpbeforefinal = ptmpnijinchalist->second;
//	cout<<"last-first"<<ptmpnijinchalist->first<<endl;
	cout<<"nilastlast"<<tmpbeforefinal<<"nilast"<<tmpfinal<<endl;
//	cout<<"size of jinchalist"<<jincha.size()<<endl;
	if(tmpbeforefinal>=0 && tmpfinal<0)
	{
	return true;
	}
	else 
	{

	return false;

	};
	 } 
	 else
		 {return false;
	 };
	

};
	

/*	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	TickData d=*(ats->pGlobalDm->getMdMana()->getTickData());

	curDiff=ma_1_5->getMAByIndex()-ma_1_10->getMAByIndex();
	preDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);  //倒数第二个均值，即上一个K线的均值
	if (preDiff<0.0 && curDiff>=0.0)
	{
		//jinChaMin=this->tc.timeToHourmin(d.getTime());
		return true;
	}
	return  false;*/

bool TestStrategy::pingxian()
{
	
	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	MAUnit* ma_1_3=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_3");
	MAUnit* ma_1_1=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_1");
	if (ma_1_1->indexList.size() < 11)
	{
		return false;
	}
	int i = 1;
	double sum = 0;
	for( i = 1; i != 11; i++)
	{
		sum = sum + abs(ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i) - ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i - 1));

	};

	double var1 = ma_1_5->getMAByIndex();
	double er = (ma_1_1->getMAByIndex() - ma_1_10->getMAByIndex())/sum;
	double sc = er * 0.6022+0.0645;
	double s2 = var1 + sc*sc*(ma_1_1->getMAByIndex() - var1);
	double s1 = ma_1_3->getMAByIndex();
	double s12 = s1-s2;
	map<int,double> ::iterator ptmpxianchalist;
	ptmpxianchalist = this->xiancha.begin();
    if(xiancha.size()>10)
	{
		xiancha.erase(ptmpxianchalist);
		xiancha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	}
	else
	{
		xiancha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	};

	if (xiancha.size()>3)
	{
	ptmpxianchalist = xiancha.end();
	ptmpxianchalist --;
	ptmpxianchalist --;
	double tmpfinal = ptmpxianchalist->second;
	ptmpxianchalist --;
	double tmpbeforefinal = ptmpxianchalist->second;
	
//	cout<<"size of pingxianlist"<<xiancha.size()<<endl;
	if(tmpbeforefinal > tmpfinal)
	{
		return true;
		cout<<"出现了pingxian true"<<endl;
	}
	else 
	{
	return false;
	}
	}
	else{ 
		return false;
	};
};

bool TestStrategy::nipingxian()
{
	
	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");
	MAUnit* ma_1_3=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_3");
	MAUnit* ma_1_1=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_1");
	if (ma_1_1->indexList.size() < 11)
	{
		return false;
	}
	int i = 1;
	double sum = 0;
	for( i = 1; i != 11; i++)
	{
		sum = sum + abs(ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i) - ma_1_1->getMAByIndex(ma_1_1->indexList.size() - i - 1));

	};

	double var1 = ma_1_5->getMAByIndex();
	double er = (ma_1_1->getMAByIndex() - ma_1_10->getMAByIndex())/sum;
	double sc = er * 0.6022+0.0645;
	double s2 = var1 + sc*sc*(ma_1_1->getMAByIndex() - var1);
	double s1 = ma_1_3->getMAByIndex();
	double s12 = s1-s2;
	map<int,double> ::iterator ptmpnixianchalist;
	ptmpnixianchalist = this->nixiancha.begin();
    if(nixiancha.size()>10)
	{
		nixiancha.erase(ptmpnixianchalist);
		nixiancha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	}
	else
	{
		nixiancha.insert(pair <int,double> (tc.timeToHourmin(ss.getTime()),s12));
	};

	if (nixiancha.size()>3)
	{
	ptmpnixianchalist = nixiancha.end();
	ptmpnixianchalist --;
	ptmpnixianchalist --;
	double tmpfinal = ptmpnixianchalist->second;
	ptmpnixianchalist --;
	double tmpbeforefinal = ptmpnixianchalist->second;
	
//	cout<<"size of pingxianlist"<<xiancha.size()<<endl;
	if(tmpbeforefinal < tmpfinal)
	{
		return true;
		cout<<"出现了nipingxian true"<<endl;
	}
	else 
	{
	return false;
	}
	}
	else{ 
		return false;
	};
};


void TestStrategy::run(){
	


	MAUnit* ma_1_5=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_5");
	MAUnit* ma_1_10=(MAUnit*)ats->pGlobalDm->getIndexMana()->getIndexUnit("ma_1_10");

	ss = *(ats->pGlobalDm->getMdMana()->getTickData());
	

	Bar* bar = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar1" + ss.getSec()));
	/*Bar* bar5 = (Bar*)(ats->pGlobalDm->getIndexMana()->getIndexUnit("Bar5" + ss.getSec()));*/

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
	
	int curBarIndex=bar->getLastBarNum(); //小周期bar index
	if(curBarIndex<14){  //当当前bar小于10+1时，不可能存在5周期均线和10周期均线金叉
		return;
	}

	
	if(ss.getSec().compare(ats->espi->Contract)!=0)
	{
		return;
	}

	bool isFirst=bar->getIsFirst();
//	cout<<"正在运行策略"<<endl;
/*	if(isFirst && ma_1_5->indexList.size()>2 && ma_1_10->indexList.size()>2){  //当此tick为当前bar的第一个tick时，判断前一个bar是否金叉
		
		double cur5=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2);
		double cur10=ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		curDiff=cur5 - cur10;
		double pre5=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3);
		double pre10=ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		preDiff=pre5 - pre10;  //倒数第二个均值，即上一个K线的均值

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
	*/
	this-> inJincha = jinCha(); 
	if(inJincha && !Bopen /*&& dist_jinCha>3*/){
		cout<<"inJinCha!!"<<endl;
		double current=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-1)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-1);
		double first=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		double second=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		double third=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-4)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-4);
//		if(first<a && second<a && third<a && current>b) {
			
			Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getAskPrice(),Buy, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or);
			cout<<"put order!!"<<endl;
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" inJinCha, Open Long Order, Buy, "+ or->getString());
			Bopen=true;
//		}
	}


	double diff=((BarValue*)bar->getIndex())->getDiff(); 

	
	//平仓策略（新平仓）!!!
	if (this->pingxian() && curBarIndex>0 && Bopen)
	{
		cout<<"平仓出现"<<endl;
		Order*  or2 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getBidPrice(),Sell, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or2);
			cout<<"put order to cover!!"<<endl;
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" inJinCha, Open Long Order, Buy, "+ or2->getString());
	Bopen = false;
		
	
	};

	//sell策略
	if(this->nijinCha() && !Sopen /*&& dist_jinCha>3*/){
		cout<<"inNiJinCha!!"<<endl;
		double current=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-1)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-1);
		double first=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
		double second=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
		double third=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-4)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-4);
//		if(first<a && second<a && third<a && current>b) {
			
			Order*  or = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getBidPrice(),Sellshort, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or);
			cout<<"put sell order!!"<<endl;
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" inJinCha, Open Long Order, Buy, "+ or->getString());
			Sopen=true;
//		}
	}


//	double diff=((BarValue*)bar->getIndex())->getDiff(); 

	
	//SELL平仓策略（新平仓）!!!
	if (this->nipingxian() && curBarIndex>0 && Sopen)
	{
		cout<<"逆平仓出现"<<endl;
		Order*  or2 = new Order(strategyID, ats->pGlobalDm->getOrdersMana()->getLastOrderID(), 
				ss.sec, ss.getAskPrice(),Buytocover, 0, 1, ss.getDate(), ss.getTime(),
				ats->espi->exchange,algomanager->getAccountMana());
			esSendOrder(or2);
			cout<<"put buy order to cover!!"<<endl;
			ats->pGlobalDm->keepRecord(HisOrder, ss.getTime()+" inJinCha, Open Long Order, Buy, "+ or2->getString());
	Sopen = false;
		
	
	};
/*老平仓在此
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
	老平仓结束*/


	////继续开仓条件判断
	//if(isFirst){
	//	double beforeLastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-3)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-3);
	//	double lastDiff=ma_1_5->getMAByIndex(ma_1_5->indexList.size()-2)-ma_1_10->getMAByIndex(ma_1_10->indexList.size()-2);
	//	if(beforeLastDiff<0.0 && lastDiff>=0.0){  //小周期金叉
	//		smallBarNum=bar->getLastBarNum();
	//		bigBarNum=bar5->getLastBarNum();
	//		smallJinCha=true;
	//	}else if(beforeLastDiff>0.0 && lastDiff<=0.0){
	//		smallJinCha=false;  //小周期死叉
	//	}
	//}

	//if(bar5->getIsFirst()){
	//	if(curBarIndex5<11){
	//		return;
	//	}
	//	double beforeLastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-3)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-3);
	//	double lastDiff=ma_5_5->getMAByIndex(ma_5_5->indexList.size()-2)-ma_5_10->getMAByIndex(ma_5_10->indexList.size()-2);
	//	if(beforeLastDiff<0.0 && lastDiff>=0.0){  //大周期发生了金叉，判断其包含的小周期是否处于金叉状态
	//		int bigBarNum_cur=bar5->getLastBarNum();
	//		if((bigBarNum_cur-bigBarNum)==1 && smallJinCha){
	//			noClose=true;  //设置可平仓flag为false
	//		}
	//	}else if(beforeLastDiff>0.0 && lastDiff<=0.0){  //大周期发生了死叉，设置可平仓flag为true
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