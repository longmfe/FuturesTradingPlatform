#pragma once
#include "indexunit.h"
#include <string>
#include <list>
#include "TickData.h"
using namespace std;
class DefaultIndexUnit :
	public IndexUnit
{
public:
	DefaultIndexUnit(void);
	~DefaultIndexUnit(void);

private:
	string name ;
    list< double > indexListOpen;
    list< double > indexListClose;
    list< double > indexListHigh;
    list< double > indexListLow;
    int len ;
    int min;
    int iInitHHMM;

    int lastMin  ;
    double open ;
    double close ;
    double high ;
    double low ;
    
public:
	double inValidV;

    


    //ÿ��BAR����min����Ϊ����
     DefaultIndexUnit(int min, int initHHMM);

   
    //Ĭ�Ϸ���open��ָ�꣬��������
    double getIndex();

   
    //֧��������������-1������һ����-2�����ڶ�����0������һ����1�����ڶ������Դ����ƣ�����ͬ��
    double getIndex(int index) ;
    
    double getIndexOpen() ;


    double getIndexOpen(int index)  ;
    
    double getIndexClose() ;

    double getIndexClose(int index)  ;
    
    double getIndexHigh() ;

    double getIndexHigh(int index) ;
    
    double getIndexLow() ;

    double getIndexLow(int index)  ;
 
    void computeIndex(TickData sec)  ;

    int timeToHourmin(string time) ;

  
    string getIUID() ;

   
    void clear()  ;

               
    void addIndexDetails(IndexDetailUnit idu)  ;
	

};

