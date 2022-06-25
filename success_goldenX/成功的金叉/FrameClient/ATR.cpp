#include "StdAfx.h"
#include "ATR.h"

ATR::ATR(string sec, int num, AlgoTSEng* ats)
{
		name = "ATR";	
		this->sec = sec;
		this->daynum = num;
		exchangeName = "Not_CFFEX";
		this->ats1 = ats;
		date = "default";		

		len = 0;
		toolsClass tc;
		name += tc.IntToString(num) + sec;

		computeIndex();
}

ATR::~ATR(void){

}

string ATR::getIUID() {
	return name;	
};

IndexValueBase* ATR::getIndex() {
	map<string ,double >::iterator it = ATRmap.end();
    return (IndexValueBase*)(&it->second);
};

IndexValueBase* ATR::getIndex(int index) {

	if (len + index < 0) {
		index = 0;
		return (IndexValueBase*)(&index);
	}
	int tmp = (len + index) % len;
	int num;
	map<string ,double >::iterator it = ATRmap.begin();
	for(num = 0; num < tmp; num++, it++) ;
	return (IndexValueBase*)(&it->second);
};

double ATR::getYesValue(){
	return yesATR;
}

void ATR::computeIndex(){
	bd = (BarDaily*)(ats1->pGlobalDm->getIndexMana()->getIndexUnit("BarDaily" + this->sec));
	if(bd->getlen() < 3+daynum) return;

	for(int j=0; j<daynum; j++){
		BarValue* bv1 =(BarValue*) bd->getIndex(-2-j);
		BarValue* bv2 =(BarValue*) bd->getIndex(-3-j);
		double tr =max((max((bv1->high - bv1->low),abs(bv2->close - bv1->high))),(abs(bv2->close - bv1->low)));
		TrueRange.push_back(tr);
	}

	if(TrueRange.size() < daynum) return;

	int i=0; double summation = 0;
	list<double>::reverse_iterator it = TrueRange.rbegin(); 
	for(;i < daynum ;it++,i++){
		summation += *it;
	}
	//ATRmap.insert(pair<string,double>(this->sec, summation / daynum));
	//		len++;
	yesATR = summation / daynum;
}

void ATR::computeIndex(TickData ss){
	if(strcmp(ss.sec.c_str(), this->sec.c_str())!=0)
	{
		return;
	}
	toolsClass tc;
	int tmp = timeToHourmin(ss.getTime());

	if(strcmp(this->exchangeName.c_str(),"CFFEX")==0)
	{
		if(tmp >= 1515)
		{
			return;
		}
		if((tmp >= 1130)&&(tmp <1300))
		{
			return;
		}
		if(tmp < 915)
		{
	       return;
		}
	}
	else 
	{
		if(tmp >= 1500)
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
	}

	bd = (BarDaily*)(ats1->pGlobalDm->getIndexMana()->getIndexUnit("BarDaily" + ss.getSec()));

	if(strcmp(ss.getDate().c_str(),this->date.c_str()) != 0){
		//if(bd->getIndex(-1) == 0 || bd->getIndex(-2) == 0){
		this->date = ss.getDate();
		if(bd->getlen() <= 2){
			return;
		}else{		
			BarValue* bv1 =(BarValue*) bd->getIndex(-1);
			BarValue* bv2 =(BarValue*) bd->getIndex(-2);
			double tr =max((max((bv1->high - bv1->low),abs(bv2->close - bv1->high))),(abs(bv2->close - bv1->low)));
			TrueRange.push_back(tr);

			if(TrueRange.size() < daynum) return;

			int i=0; double summation = 0;
			list<double>::reverse_iterator it = TrueRange.rbegin(); 
			for(;i < daynum ;it++,i++){
				summation += *it;
			}
			ATRmap.insert(pair<string,double>(ss.getDate(), summation / daynum));
			len++;
		}	
	}
}

int ATR::timeToHourmin(string time) {
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
};

void ATR::addIndexDetails(IndexDetailUnit idu)
{

};
void ATR::clear() {
	//lastMin = 0; 
};	

