#pragma once
#include <string>
using namespace std;
class contractDetail
{
public:
	contractDetail(void);
	~contractDetail(void);

	///��С�䶯�۸�
	double PriceTick;
	///��ͣ���
	double	UpperLimitPrice;
	///��ͣ���
	double	LowerLimitPrice;

	///���¼�
	double lastPrice;

	string conName;
};

