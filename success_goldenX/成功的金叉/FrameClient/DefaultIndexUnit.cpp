#include "StdAfx.h"
#include "DefaultIndexUnit.h"


DefaultIndexUnit::DefaultIndexUnit(int min, int initHHMM){
         
};

DefaultIndexUnit::~DefaultIndexUnit(void)
{
	
}
   
//Ĭ�Ϸ���open��ָ�꣬��������
double DefaultIndexUnit::getIndex(){
       return 1.0;	
};

   
//֧��������������-1������һ����-2�����ڶ�����0������һ����1�����ڶ������Դ����ƣ�����ͬ��
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
