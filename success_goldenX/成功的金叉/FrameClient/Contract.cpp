#include "StdAfx.h"
#include "Contract.h"


Contract::Contract(void)
{

	isNULL = false;
	 //��ԼID
    conid = "N/A";
    //������
     exchange = "N/A";
    //��С�仯��λ
     minPrice = 10.0;
    //��������
     commissionRate = 10.0;
    //��֤����
     ratioOfmargin = 0.1;
    //��Լ����
    multy = 10;
    //���
     preSettlement = 0.0;
    //������
      openint = 0.0;
    //������
    tradeVol = 0.0;

}
//ID,��С�䶯��minMov������Լ������point value���������ѣ���֤��
Contract::Contract(string id, double minp, int mul, double comm, double ratio)
{
	    //��ԼID
    conid = "N/A";
    //������
     exchange = "N/A";
    //��С�仯��λ
     minPrice = 10.0;
    //��������
     commissionRate = 10.0;
    //��֤����
     ratioOfmargin = 0.1;
    //��Լ����
    multy = 10;
    //���
     preSettlement = 0.0;
    //������
      openint = 0.0;
    //������
    tradeVol = 0.0;
	 multy = mul;
        conid = id;
        minPrice = minp;
        commissionRate = comm;
        ratioOfmargin =ratio;

}

Contract::~Contract(void)
{
}
 
void Contract::setMinPrice(double value){
    minPrice = value;
}


    string Contract::getConid(){
    return conid;
}

double Contract::getMinPrice(){
    return minPrice;
}

void Contract::setCommissionRate(double value){
    commissionRate =value;
}

double Contract::getCommissionRate(){
    return commissionRate;
}

void Contract::setRatioOfmargin(double value){
    ratioOfmargin = value;
}

double Contract::getRatioOfmargin(){
    return ratioOfmargin;
}

void Contract::setMulty(int value){
    multy = value;
}

double Contract::getMulty(){
    return multy;
}
	
