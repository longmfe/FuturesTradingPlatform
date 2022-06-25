#include "StdAfx.h"
#include "HighIndexUnit.h"
#include "DoubleIndexValue.h"
HighIndexUnit::HighIndexUnit(void)
{

}
HighIndexUnit::~HighIndexUnit(void)
{

}
HighIndexUnit::HighIndexUnit(int min, int initHHMM, string sec)
{

}
	
int HighIndexUnit::timeToHourmin(string time) 
{
	return 1;
}
list<int> HighIndexUnit:: GetBarSeq(int InitTime,int cycle,string exName)
{
	list<int> i;
	return i;
}
int  HighIndexUnit::TimeInc(int itime,int cycle)
{
	return 1;
}
void  HighIndexUnit::computeIndex(TickData sec)
{

}
    

//only for client side
    
string  HighIndexUnit::getIUID()
{
	return "getIUID";
}
//only for client side
   
IndexValueBase* HighIndexUnit::getIndex()
{
	DoubleIndexValue* ivb = new DoubleIndexValue(123);
	return ivb;
}
IndexValueBase* HighIndexUnit::getIndex(int index)
{
	DoubleIndexValue* ivb = new DoubleIndexValue(1);
	return ivb;
}	
	//only for client side
int HighIndexUnit::getSize()
{
	return 123;
}
//only for client side
 
void HighIndexUnit::addIndexDetails(IndexDetailUnit idu)
{

}
//only for client side
 
void HighIndexUnit::clear()
{

}