#include "StdAfx.h"
#include "DefaultIndexUnit.h"


DefaultIndexUnit::DefaultIndexUnit(int min, int initHHMM){
         
};

DefaultIndexUnit::~DefaultIndexUnit(void)
{
	
}
   
//默认返回open的指标，以下类似
double DefaultIndexUnit::getIndex(){
       return 1.0;	
};

   
//支持正负数索引，-1倒数第一个，-2倒数第二个，0正数第一个，1正数第二个，以此类推，以下同此
double DefaultIndexUnit::getIndex(int index) {
        return 1.0;
};
    
double DefaultIndexUnit::getIndexOpen(){
       	return 1.0;
};


double DefaultIndexUnit::getIndexOpen(int index) {
       return 1.0;
};
    
double DefaultIndexUnit::getIndexClose(){
       return 1.0;
};

double DefaultIndexUnit::getIndexClose(int index) {
       return 1.0;
};
    
double DefaultIndexUnit::getIndexHigh(){
       return 1.0;	
};

double DefaultIndexUnit::getIndexHigh(int index) {
       return 1.0;      
};
    
double DefaultIndexUnit::getIndexLow(){
       return 1.0;	
};

double getIndexLow(int index) {
      return 1.0;
};
 
void DefaultIndexUnit::computeIndex(TickData sec) {
       
};

int DefaultIndexUnit::timeToHourmin(string time){
//xx:xx:xx.xxx
      return 1;
};

  
string DefaultIndexUnit::getIUID() {
       return "123";
};

   
void DefaultIndexUnit::clear() {
       
};

               
void DefaultIndexUnit::addIndexDetails(IndexDetailUnit idu) {
        
};
