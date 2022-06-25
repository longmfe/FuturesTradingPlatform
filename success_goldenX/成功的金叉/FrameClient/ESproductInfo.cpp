#include "StdAfx.h"
#include "ESproductInfo.h"
#include "toolsClass.h"
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

ESproductInfo::ESproductInfo()
{
	this->strSep = ",";
	this->FieldsCount = 13;
}
ESproductInfo::~ESproductInfo(void)
{

}
bool ESproductInfo::string2object(string str)
{

	if(str.compare("")==0)
	{
		cout << "string is empty [ESproductInfo::string2object]" << endl;
        return false;
	}
	else
	{
		vector<string>  proinfo = this->tc.split(str,this->strSep, "<null>");
		if(proinfo.size()<this->FieldsCount)
		{
			cout << "lack of fields,pls check the instancecfg.csv  [ESproductInfo::string2object],str :" << str << endl;
			return false;
		}
		else
		{
			for(int k=0; k<proinfo.size();k++)
			{
	/*
					0.Name,品种名称
					1.Contract,合约名称
					2.Vol
					3.Strategy
					4.tn
					5.PosMult
					6.LossPrs;
					7.tkPrs;
					8.minMove
					9.pointValue
					10.commission
					11.ratioOfmargin
					12.proportion
					13.exchange
					14.tradingSessionString
	*/
				this->Name = proinfo[0];
				this->Contract = proinfo[1];
				this->Vol = proinfo[2];
		        this->Strategy = proinfo[3];
			    this->tn = proinfo[4];
				this->PosMult = proinfo[5];

				this->LossPrs = proinfo[6];
				this->tkPrs = proinfo[7];
				this->minMove = proinfo[8];
				this->pointValue = proinfo[9];
				this->commission = proinfo[10];
				
				this->ratioOfmargin = proinfo[11];
				this->proportion = proinfo[12];
				this->exchange = proinfo[13];

				this->tradingSessionString = proinfo[13];
			}
		    return true;
		}       
	}
}
