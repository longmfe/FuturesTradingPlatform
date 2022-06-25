#pragma once
#include <string>
using namespace std;
class contractDetail
{
public:
	contractDetail(void);
	~contractDetail(void);

	///最小变动价格
	double PriceTick;
	///涨停板价
	double	UpperLimitPrice;
	///跌停板价
	double	LowerLimitPrice;

	///最新价
	double lastPrice;

	string conName;
};

