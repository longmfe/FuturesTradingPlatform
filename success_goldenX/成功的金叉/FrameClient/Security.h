#pragma once
#include <string>
using namespace std;
class CSecurity
{
public:
	CSecurity(void);
	~CSecurity(void);
	

	string sec  ;
	string exchange  ;
	string time;
    string date;
	
	double AskPrice ;
	double AskVol;
	double BidPrice;
	double BidVol;
	//if lastprice=null,lastprice=presettlement
	double LastPrice;
	double LastVol;
	
    double PreSettlement;
        
	double OpenInt;
	double TradeVol;
	
	CSecurity(string name);
	
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

};

