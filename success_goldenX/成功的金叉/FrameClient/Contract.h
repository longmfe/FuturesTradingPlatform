#pragma once
#include <string>
using namespace std;
class Contract
{
public:
	Contract(void);
	~Contract(void);
	bool isNULL;
private:
	//��ԼID
    string conid ;
    //������
    string exchange;
    //��С�仯��λ
    double minPrice ;
    //��������
    double commissionRate;
    //��֤����
    double ratioOfmargin;
    //��Լ����
    int multy ;
    //���
    double preSettlement;
    //������
    double openint ;
    //������
    double tradeVol ;
public:
    Contract(string id, double minp, int mul, double comm, double ratio);

   void setMinPrice(double value);


    string getConid();

    double getMinPrice();

    void setCommissionRate(double value);

    double getCommissionRate();

    void setRatioOfmargin(double value);

    double getRatioOfmargin();

    void setMulty(int value);

     double getMulty();
};

