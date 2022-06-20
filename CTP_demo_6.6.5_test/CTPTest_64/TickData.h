#pragma once
#include <string>
using namespace std;
class TickData
{
public:
	TickData(void);
	~TickData(void);
	string sec;
	string exchange;
	string time ;
    string date;
	
	double AskPrice;
	double AskVol;
	double BidPrice;
	double BidVol;
	//if lastprice=null,lastprice=presettlement
	double LastPrice;
	double LastVol ;
	
    double PreSettlement;
        
	double OpenInt;
	double TradeVol;
	bool isNULL;
		string separator;
public:
	TickData(string name);
	string getSec();
	
	void setAskPrice(double value);
	
	double getAskPrice();
	
	void setAskVol(double value);
	
	double getAskVol();
	void setBidPrice(double value);
	
	double getBidPrice();
	
	void setBidVol(double value);
	
	double getBidVol();
	
	void setLastPrice(double value);
	
	double getLastPrice();
	void setLastVol(double value);
	
	double getLastVol();
	void setOpenInt(double value);
	
	double getOpenInt();
	
	void setTradeVol(double value);
	
	double getTradeVol();
	
	void setTime(string value);
	string getTime();
        
    void setPreSettlement(double value);
	
	double getPreSettlement();
    void setDate(string value);
	
	string getDate();
	string Object2String();
	void fill(TickData ss);
};

