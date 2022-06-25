#include "StdAfx.h"
#include "OpenIndexUnit.h"
#include "IndexValueBase.h"
#include "DoubleIndexValue.h"

OpenIndexUnit::OpenIndexUnit(int min, int initHHMM, string sec){

}

int OpenIndexUnit::timeToHourmin(string time){
	return 123;
}
list<int>  OpenIndexUnit::GetBarSeq(int InitTime,int cycle,string exName){
	return barSeq;
}
int  OpenIndexUnit::TimeInc(int itime,int cycle){
	return 1;
}
void OpenIndexUnit::computeIndex(TickData sec) {
	
}

//only for client side
 
string OpenIndexUnit::getIUID(){
	return "123";
}
	
//only for client side
 
// 
IndexValueBase* OpenIndexUnit::getIndex(){
	DoubleIndexValue* ivb = new DoubleIndexValue(1);
	return ivb;
}
	
 
IndexValueBase*  OpenIndexUnit::getIndex(int index) {
DoubleIndexValue* ivb = new DoubleIndexValue(1);
	return ivb;
}
//only for client side
int  OpenIndexUnit::getSize(){
   return 1;
}
//only for client side
void  OpenIndexUnit::addIndexDetails(IndexDetailUnit idu){
}
void  OpenIndexUnit::clear(){

}
	
