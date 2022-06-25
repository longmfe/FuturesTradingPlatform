#include "stdafx.h"
#include "CombBar.h"

CombBar::CombBar(int inithhmm, Bar* b1, Bar* b2, int maNum){
	this->inithhmm=inithhmm;
	this->bar1 = b1;
	this->bar2 = b2;
	this->contract1=b1->getSec();
	this->contract2=b2->getSec();
	lastMin=0;
	this->maNum=maNum;

	init();
}

void CombBar::init(){
	int initSize=bar1->getSize();
	for(int i = 0; i < initSize; i++) {
		priceDiffer=((BarValue*)bar1->getIndex(i))->close - ((BarValue*)bar2->getIndex(i))->close;
		volumeSum=((BarValue*)bar1->getIndex(i))->tradevolume + ((BarValue*)bar2->getIndex(i))->tradevolume;

		differVector.push_back(priceDiffer);
		sumVector.push_back(volumeSum);
		maDifferV.push_back(0.0);
		maSumV.push_back(0.0);
	}
}

void CombBar::computeIndex(TickData sec){
	/*if(strcmp(sec.getDate().c_str(),this->date.c_str())!=0){
		if(strcmp(this->date.c_str(),"default")==0){
			date=sec.getDate();
		}
	}*/

	//int tmp = su.timeToHourmin(sec.getTime());   //只取小时和分钟，最多4位数
	int tmp = (tc.timeToHourmin(sec.getTime()))/100;   //只取小时和分钟，最多4位数

	if (tmp < inithhmm) {
		return;
	}
	if(tmp > 1500)
	{
		return;
	}
	if((tmp >= 1130)&&(tmp <1330))
	{
		return;
	}
	if((tmp >=1015)&&(tmp <1030))
	{
		return;
	}
	if(tmp < 859)
	{
		return;
	}

	int tmpLastMin=lastMin;

	if (bar1->getLastMin() >= bar2->getLastMin())
	{
		lastMin=bar2->getLastMin();
	}else{
		lastMin=bar1->getLastMin();
	}

	bool newBar=false;
	if (lastMin>tmpLastMin)  //判断是否为新的bar
	{
		newBar=true;
	}

	if ((lastMin==bar1->getLastMin()) && (lastMin==bar2->getLastMin()))  //等两个合约bar都为同一分钟时才计算组合指标
	{
		priceDiffer=bar1->getClose()-bar2->getClose();
		volumeSum=bar1->getVolume()+bar2->getVolume();
		//priceDiffer=((BarValue*)bar1->getIndex())->close-((BarValue*)bar2->getIndex())->close;
		//volumeSum=((BarValue*)bar1->getIndex())->tradevolume-((BarValue*)bar2->getIndex())->tradevolume;

		if (newBar){
			int blength1=bar1->getSize();
			int blength2=bar2->getSize();

			cout << "==============================================" << endl << sec.getTime() << endl 
				<< "合约1: " << bar1->getSec() << ", 收盘价: " << ((BarValue*)bar1->getIndex(blength1-2))->close << ", 成交量: " << ((BarValue*)bar1->getIndex(blength1-2))->tradevolume << endl 
				<< "合约2: " << bar2->getSec() << ", 收盘价: " << ((BarValue*)bar2->getIndex(blength2-2))->close << ", 成交量: " << ((BarValue*)bar2->getIndex(blength2-2))->tradevolume << endl	
				<< "组合合约价差： " << differVector.back() << ", 量和: " << sumVector.back() << endl
				<< "组合合约平均价差： " << maDifferV.back() << ", 平均量和: " << maSumV.back() << endl;

			differVector.push_back(priceDiffer);
			sumVector.push_back(volumeSum);
			//maDifferV.push_back(0.0);
			//maSumV.push_back(0.0);
		}else{
			differVector.pop_back();
			differVector.push_back(priceDiffer);
			sumVector.pop_back();
			sumVector.push_back(volumeSum);
		}

		int combBarSize=differVector.size();
		//if (combBarSize>=maNum)  //当combBar的个数大于等于均线周期数时才开始计算移动平均值
		//{
			double maDiffSum=0.0;
			double maDiff=0.0;
			double maVolumeSum=0;
			double maVolume=0;
			for (int i=0;i<maNum;i++)
			{
				int idx=combBarSize-1-i;
				maDiffSum=maDiffSum+differVector[idx];
				maVolumeSum=maVolumeSum+sumVector[idx];
			}
			maDiff=maDiffSum/maNum;
			maVolume=maVolumeSum/maNum;
			
			if (newBar){
				maDifferV.push_back(maDiff);
				maSumV.push_back(maVolume);
			}else{
				maDifferV.pop_back();
				maDifferV.push_back(maDiff);
				maSumV.pop_back();
				maSumV.push_back(maVolume);
			}
		//}
	}
}

IndexValueBase* CombBar::getIndex(){return NULL;}
IndexValueBase* CombBar::getIndex(int index){return NULL;}

double CombBar::getDifferByIndex(int index){
	return differVector[index];
}

int CombBar::getDifferVsize(){
	return differVector.size();
}

double CombBar::getSumByIndex(int index){
	return sumVector[index];
}

void CombBar::addIndexDetails(IndexDetailUnit du){

}

string CombBar::getIUID(){
	return contract1+"_"+contract2;
}

void CombBar::clear(){
	differVector.clear();
	maDifferV.clear();
	sumVector.clear();
	maSumV.clear();
}

double CombBar::getBidPrice1(){
	return bar1->getBidPrice();
}

double CombBar::getAskPrice1(){
	return bar1->getAskPrice();
}

double CombBar::getBidPrice2(){
	return bar2->getBidPrice();//((BarValue*)bar2->getIndex(1))->close; //bar2->getBidPrice();
}

double CombBar::getAskPrice2(){
	return bar2->getAskPrice();//((BarValue*)bar2->getIndex(1))->close;  //bar2->getAskPrice();
}


