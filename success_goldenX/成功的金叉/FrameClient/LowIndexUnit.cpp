#include "StdAfx.h"
#include "LowIndexUnit.h"
#include "DoubleIndexValue.h"

LowIndexUnit::LowIndexUnit(int min, int initHHMM, string sec) {}
	
int LowIndexUnittimeToHourmin(string time){
return 1;
}
list<int>  LowIndexUnit::GetBarSeq(int InitTime,int cycle,string exName) 
{
	return barSeq;
} 
int  LowIndexUnit::TimeInc(int itime,int cycle)
{
	return 1;
} 
 
void  LowIndexUnit::computeIndex(TickData sec) {
} 
//only for client side
	 
string  LowIndexUnit::getIUID() {
return "123";
};
//only for client side
IndexValueBase*  LowIndexUnit::getIndex() {
	DoubleIndexValue* ivb = new DoubleIndexValue(123);
	return ivb;
}
//only for client side
 
IndexValueBase*  LowIndexUnit::getIndex(int index){
	DoubleIndexValue* ivb = new DoubleIndexValue(123);
	return ivb;
}
//only for client side  
int getSize(){
	return 123;
}
//only for client side
 
void LowIndexUnit::addIndexDetails(IndexDetailUnit idu) {

}
//only for client side
	 
void LowIndexUnit::clear() {

}
