#include "StdAfx.h"
#include "MAUnit.h"
#include "toolsClass.h"
#include "Bar.h"


MAUnit::MAUnit(string id, int min, int num, GlobalDataManage* p){
	name=id;
	this->min=min;
	this->num=num;
	curMAV=0.0;
	lastMin=0;
	pGlobalDm=p;
	len=0;
}

void MAUnit::computeIndex(TickData ss){
	boolean newbar = false;
	int tmp = timeToHourmin(ss.getTime());

	/*if(tmp < 915){
	return;
	}*/

	if(tmp != lastMin){

		if(tmp%min == 0){
			lastMin = tmp;
			newbar = true;
		}

		if(tmp - min >= lastMin){
			lastMin = tmp;
			newbar = true;
		}
	}

	/*if(tmp == 1130 || tmp == 1259){                  
	return;
	}*/

	toolsClass tc;
	Bar* bar = (Bar*)(pGlobalDm->getIndexMana()->getIndexUnit("Bar"+ tc.IntToString(min) + ss.getSec()));  //BarDaily要改成Bar1  by zengty
	int blength=bar->getSize();
	int lastBarNum=bar->getLastBarNum();
	double sum=0;

	//当当前bar的个数还不到均线个数时，如何处理均线值？
	if(lastBarNum<num || blength<num){
		return;
	}else{
		for (int i=0;i<num;i++)
		{
			int idx=blength-1-i;
			//cout<<"blength="<<blength<<", idx="<<idx<<", num="<<num<<", lastBarNum="<<lastBarNum<<endl;

			sum=sum+((BarValue*)bar->getIndex(idx))->close;
		}
		curMAV=sum/num;
	}

	if(!newbar){
		indexList.pop_back();
		indexList.push_back(curMAV);
	}else{
		
		indexList.push_back(curMAV);
		len++;
		cout<<"newbar, indexList size of MA is: "<<len<<", curMAV="<<curMAV<<endl;
	}

}

string MAUnit::getIUID(){
	return name;
}

double MAUnit::getCurMAV(){
	return curMAV;
}

void MAUnit::clear(){
	indexList.clear();
	len=0;
	lastMin=0;
}

void MAUnit::addIndexDetails(IndexDetailUnit idu){

}

int MAUnit::timeToHourmin(string time) {
	int result = 0;

	toolsClass tc;

	string tmp = tc.deleteSubStr(time,':');
	result = tc.stringToInt(tmp);
	string tt;
	tmp = "" + tc.IntToString(result);
	tt = tmp.substr(0, 1);
	result = tc.stringToInt(tt);
	if (result > 1) {
		tt = tmp.substr(0, 3);
		result = tc.stringToInt(tt);
	} else {
		tt = tmp.substr(0, 4);
		result = tc.stringToInt(tt);
	}
	return result;
}

IndexValueBase* MAUnit::getIndex(){return NULL;}
IndexValueBase* MAUnit::getIndex(int index){return NULL;}

double MAUnit::getMAByIndex(){
	return indexList.back();
}



double MAUnit::getMAByIndex(int index) {
	list<double>::iterator clit = indexList.begin();
	int len=indexList.size();
	for(int i = 0;i<index;i++)
	{
		clit++;
	}
	return *clit;
}




MAUnit::~MAUnit(void){}