#include "StdAfx.h"
#include "contractDetail.h"


contractDetail::contractDetail(void)
{
	PriceTick = 0;
	///涨停板价
	UpperLimitPrice= 0;
	///跌停板价
	LowerLimitPrice= 0;
	///最新价
	lastPrice= 0;
}


contractDetail::~contractDetail(void)
{


}
