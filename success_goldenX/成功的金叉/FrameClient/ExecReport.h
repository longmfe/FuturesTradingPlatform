#pragma once
#include <string>
#include "Order.h"
#include "toolsClass.h"
using namespace std;
enum Stype;
class ExecReport
{
public:
	ExecReport(void);
	~ExecReport(void);
	int orderID;
    string sec;
    double tradePrice;
    int TradeVol;
    //报单类型
    Stype type;
    int status;
    string tradetime;
    int orderRef;
	string getTradetime() ;
	void setTradetime(string tradetime)  ;

	

    string split ;
    /**
     * @return Returns the orderID.
     */
	int getOrderID() ;
    /**
     * @param orderID The orderID to set.
     */
	void setOrderID(int orderID) ;
    /**
     * @return Returns the sec.
     */
	string getSec() ;
    /**
     * @param sec The sec to set.
     */
	void setSec(string sec) ;
    /**
     * @return Returns the tradePrice.
     */
	double getTradePrice() ;
    /**
     * @param tradePrice The tradePrice to set.
     */
	void setTradePrice(double tradePrice) ;
    /**
     * @return Returns the tradeVol.
     */
	int getTradeVol() ;
    /**
     * @param tradeVol The tradeVol to set.
     */
	void setTradeVol(int tradeVol) ;
    
    
    /**
     * @return Returns the Order type.
     */
	Stype getType()  ;
    /**
     * @param tradeVol The Order type. to set.
     */
	void setType( Stype value) ;
    
	string toString();

	string StrategyID;	 
	string getStrategyID() {return StrategyID;} ;     
	void setStrategyID( string value) {StrategyID = value;};
};

