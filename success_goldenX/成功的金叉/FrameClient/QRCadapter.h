#pragma once
#include"TickData.h"
#include "BarValue.h"
#include "ESproductInfo.h"
class QRCadapter
{
public:
	    QRCadapter(ESproductInfo* qpi);
		~QRCadapter(void);
		ESproductInfo* QPI;
		void connect();
		void initReq(double x);
		int preFeedin();
		string preLoad(string tickername,string datetime,string open,string high,string low,string close,string vol,string pos,bool isEnd);

		string MDFeedin(TickData ss,double postion);
		string BarFeedin(BarValue bv,double postion,bool isEnd);
		string DoubleToString(double d);

		string minutePP(string time);

};