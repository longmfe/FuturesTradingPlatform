#include "StdAfx.h"
#include "MACD.h"

MACD::MACD(string sec, int fastLen, int slowLen, int macdLen, AlgoTSEng* ats, int min)
{
		name = "MACD";	
		this->sec = sec;
		this->fastLength = fastLen;
		this->slowLength = slowLen;
		this->macdLength = macdLen;
		exchangeName = "Not_CFFEX";
		this->ats1 = ats;
		date = "default";

		filename = "./MACD/" + sec + ".csv";

		len = 0;
		toolsClass tc;
		name += sec;

		this->min=min;

		//computeIndex();
		xF=0;
		xS=0;
		MV=0; 
		xM=0;
}

MACD::~MACD(void){
}

string MACD::getIUID() {
	return name;	
};

IndexValueBase* MACD::getIndex() {
	map<string ,double >::iterator it = MACDmap.end();
    return (IndexValueBase*)(&it->second);
};

IndexValueBase* MACD::getIndex(int index) {

	if (len + index < 0) {
		index = 0;
		return (IndexValueBase*)(&index);
	}
	int tmp = (len + index) % len;
	int num;
	map<string ,double >::iterator it = MACDmap.begin();
	for(num = 0; num < tmp; num++, it++) ;
	return (IndexValueBase*)(&it->second);
};

double MACD::getYesValue(){
	return yesMACD;
}

void MACD::computeIndex(TickData ss){
	toolsClass tc;	
	bd = (Bar*)(ats1->pGlobalDm->getIndexMana()->getIndexUnit("Bar" + tc.IntToString(min) + this->sec));  //BarDailyÒª¸Ä³ÉBar1  by zengty
	bool isFirst=bd->getIsFirst();
	
	if(bd->getSize() < fastLength + macdLength || bd->getSize() < slowLength + macdLength){
		return;
	}else{
		if(isFirst){
			string k=ss.getTime();
		}
		int i=0, j=0; 
		double MACDValue = 0;
		double AvgMACD = 0;
		double xaverageFast;
		double xaverageSlow;
		double xaverageMACD;
		double sFcactorFast = 2.0 / ( fastLength + 1 );
		double sFcactorSlow = 2.0 / ( slowLength + 1 );
		double sFcactorMACD = 2.0 / ( macdLength + 1 );
		int bdlength = bd->getSize();

		for(i = 0; i < bdlength; i++){
			if(i == 0){
				xaverageFast = ((BarValue*) bd->getIndex(i-bdlength))->close;
				xaverageSlow = ((BarValue*) bd->getIndex(i-bdlength))->close;
				MACDValue = xaverageFast - xaverageSlow;
				xaverageMACD = MACDValue;
			}else{
				xaverageFast = xaverageFast + sFcactorFast * (((BarValue*) bd->getIndex(i-bdlength))->close - xaverageFast);
				xaverageSlow = xaverageSlow + sFcactorSlow * (((BarValue*) bd->getIndex(i-bdlength))->close - xaverageSlow);
				MACDValue = xaverageFast - xaverageSlow;
				xaverageMACD = xaverageMACD + sFcactorMACD * (MACDValue - xaverageMACD);
			}
		}
		yesMACD = MACDValue - xaverageMACD;
		((BarValue*)bd->getIndex())->setDiff(yesMACD);

		xF=xaverageFast;
		xS=xaverageSlow;
		MV=MACDValue; 
		xM=xaverageMACD;
	}
}

void MACD::computeIndex(){

}

int MACD::timeToHourmin(string time) {
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

void MACD::addIndexDetails(IndexDetailUnit idu)
{

};
void MACD::clear() {
};	