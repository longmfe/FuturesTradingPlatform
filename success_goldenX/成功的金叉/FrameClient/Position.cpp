#include "StdAfx.h"
#include "Position.h"
extern ContractsManage* g_conM;
extern AlgoManager* algomanager;

Position::Position(GlobalDataManage* am)
{
	m_am = am;
	isNULL = false;
	 sellopen = 0;
    //买开仓位
    buyopen = 0;
    //卖均价
     sellAvg = 0.0;
    //买均价
     buyAvg = 0.0;
    //浮动盈亏
     floatProfit = 0.0;
    //买盈亏
    BuyProfit = 0.0;
    //卖盈亏
    SellProfit = 0.0;
    //卖开保证金
    sellMagin = 0.0;
    //买开保证金
     buyMagin = 0.0;

    sellEntryPrice = 0.0;
    buyEntryPrice = 0.0;
}


Position::~Position(void)
{
}
Position::Position(string s, GlobalDataManage* am)
{
	m_am = am;
    sec = s;
	isNULL = false;
	 sellopen = 0;
    //买开仓位
    buyopen = 0;
    //卖均价
     sellAvg = 0.0;
    //买均价
     buyAvg = 0.0;
    //浮动盈亏
     floatProfit = 0.0;
    //买盈亏
    BuyProfit = 0.0;
    //卖盈亏
    SellProfit = 0.0;
    //卖开保证金
    sellMagin = 0.0;
    //买开保证金
     buyMagin = 0.0;

    sellEntryPrice = 0.0;
    buyEntryPrice = 0.0;
}
Position::Position(Order or, GlobalDataManage* am)
{
	m_am = am;
	isNULL = false;
	sellopen = 0;
    //买开仓位
    buyopen = 0;
    //卖均价
     sellAvg = 0.0;
    //买均价
     buyAvg = 0.0;
    //浮动盈亏
     floatProfit = 0.0;
    //买盈亏
    BuyProfit = 0.0;
    //卖盈亏
    SellProfit = 0.0;
    //卖开保证金
    sellMagin = 0.0;
    //买开保证金
    buyMagin = 0.0;
    sellEntryPrice = 0.0;
    buyEntryPrice = 0.0;
	sec = or.getSec();	
	if(or.getType() == Stype::Buy){
			buyopen = or.getTradeVolume();
			buyAvg = or.getTradePrice();
			buyMagin += or.getMargin();
			buyEntryPrice = or.getTradePrice();
	}

	if(or.getType() == Stype::Sellshort){
			sellopen = or.getTradeVolume();
			sellAvg = or.getTradePrice();
			sellMagin += or.getMargin();
			sellEntryPrice = or.getTradePrice();
	}
            
}
double Position::getSellEntryPrice()
{
    return sellEntryPrice;
}
double Position::getBuyEntryPrice()
{
        return buyEntryPrice;
}
void Position::addPosition(Order or)
{
	if(strcmp(sec.c_str(),or.getSec().c_str())!=0){
		cout<<"Position::addPositon: sec is not equal"<<endl;
		return ;
	}

	double tmp1;
	double tmp2;

	if(or.getType() ==  Stype::Buy)
	{
		if(buyopen == 0)
		{
			buyEntryPrice = or.getTradePrice();
		}
            
		tmp1 = buyopen * buyAvg;
		tmp2 = or.getTradeVolThisTime()* or.getTradePrice();
		buyopen += or.getTradeVolThisTime();
		//00001 end modify by cf.or.getTradeVolume()--->or.getTradeVolTHisTime().because getTradeVolume() get all nums of traded vol
		buyAvg = (tmp1 + tmp2)/buyopen;

		buyMagin += or.getMargin();
		//double tmp = AlgoTSEng.GlobalDm.getAccountMana().getFroneComm();		
		//double tmp11 = AlgoTSEng.GlobalDm.getAccountMana().getFronemargin();	


		//AlgoTSEng.GlobalDm.getAccountMana().setFroneComm(tmp - or.getFroneCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setFronemargin(tmp11 - or.getFroneMargin());

		//double t1 = AlgoTSEng.GlobalDm.getAccountMana().getCommision();
		//double t2 = AlgoTSEng.GlobalDm.getAccountMana().getMargin();

		//AlgoTSEng.GlobalDm.getAccountMana().setCommision(t1 + or.getCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setMargin(t2 + or.getMargin());

		//double tmp2 = AlgoTSEng.GlobalDm.getAccountMana().getFreeMoney();
		//double tmp3 = tmp2 - tmp - tmp1;	
	}

	if(or.getType() ==  Stype::Sell){
		tmp1 = buyopen*buyAvg;
		//00001 modify getTradeVolume --->getTradeVolThisTime
		//tmp2 = or.getTradeVolume()* or.getTradePrice();
		tmp2 = or.getTradeVolThisTime()* or.getTradePrice();
		double tmp3 = (tmp2 - or.getTradeVolThisTime() * buyAvg) *  (g_conM->getContractByID(sec)->getMulty());                        
		int tt1 = buyopen;
		double tt2 = buyAvg;
		buyopen -= or.getTradeVolThisTime();
		//00001 end 

		if(buyopen == 0){
			buyAvg = 0;
			BuyProfit = 0;
			floatProfit = BuyProfit + SellProfit;
		}
		else{
			//buyAvg = (tmp1 - tmp2)/buyopen;
		}

		buyMagin = (tt1 * tt2 - buyopen * buyAvg) * (g_conM->getContractByID(sec)->getMulty())
					*  (g_conM->getContractByID(sec)->getRatioOfmargin());// * or.getTradeVolume();
		//zheshi fudong de  .
		//BuyProfit += tmp3;
		//pingcang
		//double tmp4 = AlgoTSEng.GlobalDm.getAccountMana().getClosePositonProfits();
		//AlgoTSEng.GlobalDm.getAccountMana().setClosePositonProfits(tmp4 + tmp3);
		algomanager->getAccountMana()->addClosePositonProfits(tmp3);


		//tmp4 = AlgoTSEng.GlobalDm.getAccountMana().getCommision();
		//AlgoTSEng.GlobalDm.getAccountMana().setCommision(tmp4 + or.getCommision());

		//double t1 = AlgoTSEng.GlobalDm.getAccountMana().getCommision();
		double t2 = algomanager->getAccountMana()->getMargin();

		//AlgoTSEng.GlobalDm.getAccountMana().setCommision(t1 + or.getCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setMargin(t2 - buyMagin);
		algomanager->getAccountMana()->addMargin(0 - buyMagin);

		//double tmp11 = AlgoTSEng.GlobalDm.getAccountMana().getFroneComm();
		//AlgoTSEng.GlobalDm.getAccountMana().setFroneComm(tmp11 - or.getFroneCommision());

		//keyong
		//tmp4 = AlgoTSEng.GlobalDm.getAccountMana().getFreeMoney();
		//AlgoTSEng.GlobalDm.getAccountMana().setFreeMoney(tmp4  + AlgoTSEng.GlobalDm.getAccountMana().getClosePositonProfits() - buyMagin);			

	}


	if(or.getType() == Stype::Sellshort){
		if(sellopen == 0){
			sellEntryPrice = or.getTradePrice();
		}
		tmp1 = sellopen*sellAvg;
		//00001 modify getTradeVolume --->getTradeVolThisTime
		tmp2 = or.getTradeVolThisTime()* or.getTradePrice();
		sellopen += or.getTradeVolThisTime();
		sellAvg = (tmp1 + tmp2)/sellopen;
		//00001 end
		sellMagin += or.getMargin();

		//double tmp = AlgoTSEng.GlobalDm.getAccountMana().getFroneComm();		
		//double tmp11 = AlgoTSEng.GlobalDm.getAccountMana().getFronemargin();	

		//double tmp2 = AlgoTSEng.GlobalDm.getAccountMana().getFreeMoney();
		//double tmp3 = tmp2 - tmp - tmp1;		
		//AlgoTSEng.GlobalDm.getAccountMana().setFroneComm(tmp - or.getFroneCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setFronemargin(tmp11 - or.getFroneMargin());

		//double t1 = AlgoTSEng.GlobalDm.getAccountMana().getCommision();
		//double t2 = AlgoTSEng.GlobalDm.getAccountMana().getMargin();

		//AlgoTSEng.GlobalDm.getAccountMana().setCommision(t1 + or.getCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setMargin(t2 + or.getMargin());
	}

	if(or.getType() == Stype::Buytocover){
		tmp1 = sellopen * sellAvg;
		//00001 modify getTradeVolume --->getTradeVolThisTime
		tmp2 = or.getTradeVolThisTime()* or.getTradePrice();
		double tmp3 = ( or.getTradeVolThisTime() * sellAvg - tmp2 )* (g_conM->getContractByID(sec)->getMulty());

		int tt1 = sellopen;
		double tt2 = sellAvg;

		sellopen -= or.getTradeVolThisTime();
			//end
		if(sellopen == 0){
			sellAvg = 0;
			SellProfit = 0;
			floatProfit = BuyProfit + SellProfit;
		}
		else{
			//sellAvg = (tmp1 - tmp2)/sellopen;
		}

		sellMagin = (tt1 * tt2 - sellopen * sellAvg) * (g_conM->getContractByID(sec)->getMulty())
					*(g_conM->getContractByID(sec)->getRatioOfmargin());// * or.getTradeVolume();

		//SellProfit += tmp3;

		//pingcang
		//double tmp4 = AlgoTSEng.GlobalDm.getAccountMana().getClosePositonProfits();
		//AlgoTSEng.GlobalDm.getAccountMana().setClosePositonProfits(tmp4 + tmp3);
		algomanager->getAccountMana()->addClosePositonProfits(tmp3);

		//double t1 = AlgoTSEng.GlobalDm.getAccountMana().getCommision();
		double t2 = algomanager->getAccountMana()->getMargin();

		//AlgoTSEng.GlobalDm.getAccountMana().setCommision(t1 + or.getCommision());
		//AlgoTSEng.GlobalDm.getAccountMana().setMargin(t2 - sellMagin);
		algomanager->getAccountMana()->addMargin(0 - sellMagin);
		//double tmp11 = AlgoTSEng.GlobalDm.getAccountMana().getFroneComm();
		//AlgoTSEng.GlobalDm.getAccountMana().setFroneComm(tmp11 - or.getFroneCommision());

		//keyong
		//tmp4 = AlgoTSEng.GlobalDm.getAccountMana().getFreeMoney();
		//AlgoTSEng.GlobalDm.getAccountMana().setFreeMoney(tmp4  + AlgoTSEng.GlobalDm.getAccountMana().getClosePositonProfits() - sellMagin);	
	}
        
	//add output
	toolsClass tc;
	/*
	string str = "Strategy ID:" + or.StrategyName + ",Sec:" + sec + ",Date" + or.Date+ "," + "Direction:" + tc.doubleToString(or.getType()) + "," + "Price:" + tc.doubleToString(or.getTradePrice())
				+ ","
				+ "lots" + tc.doubleToString(or.getTradeVolume())+ "," + "TradeTime:" + or.getTradeTime() + "," + tc.doubleToString(pGlobalDm->getAccountMana()->getEquity())
				+ "," +tc.doubleToString(pGlobalDm->getAccountMana()->getFreeMoney());
	*/
	string split = ",";
	string di = "";
	if(or.getType() == Buy)
	{
	        di = "buyopen";
	}
	else if(or.getType() == Buytocover)
	{
	      di = "buytocover";
	} 
	else if(or.getType() == Sell)
	{
	    di = "sellcover";
	}
	else if(or.getType() == Sellshort)
	{
	    di = "sellshort";
	}
	string str =sec + split+ di + split + tc.doubleToString(or.getSendPrice()) + split
		+ tc.doubleToString(or.getTradeVolume()) + split + or.getTradeTime() + split + tc.doubleToString(algomanager->getAccountMana()->getEquity())
		+ split + tc.doubleToString(algomanager->getAccountMana()->getFreeMoney());
		

	m_am->keepRecord(FileType::TradeDetail, str);
}
int Position::getSellopen()
{
        return sellopen;
}
int Position::getBuyopen()
{
        return buyopen;
}
double Position:: getBuyAvg()
 {
        return buyAvg;
}
double  Position::getSellAvg()
{
        return sellAvg;
}
string  Position::getSec()
{
        return sec;
}
double  Position::getFloatProfit()
{
        return floatProfit;
}
double  Position::getBuyProfit()
{
        return BuyProfit;
}
double  Position::getSellProfit()
{
        return SellProfit;
}
void  Position::computeProfit(double price)
{
	if(g_conM->getContractByID(sec) == NULL)
	{
        return;
    }
    BuyProfit = buyopen * (price - buyAvg) * g_conM->getContractByID(sec)->getMulty();;
    SellProfit = sellopen * (sellAvg - price) * g_conM->getContractByID(sec)->getMulty();;

    floatProfit = BuyProfit + SellProfit;

}

void Position::addExecReport(ExecReport er)
{
	if(strcmp(er.getSec().c_str(),sec.c_str())!=0){//if(!sec.equals(er.getSec()))
        cout<<"Position::addPositon: sec is not equal"<<endl;
        return ;
    }

    if(er.getType() ==  Stype::Buy){
        if(buyopen == 0){
            buyEntryPrice = er.getTradePrice();
        }else
		{
			buyEntryPrice = (buyEntryPrice*buyopen+er.getTradePrice()*er.getTradeVol())/(buyopen+er.getTradeVol());
			buyAvg = buyEntryPrice;
		}
            
        buyopen += er.getTradeVol();           
    }

    if(er.getType() == Stype::Sell){  
		double tmpPretotalPrice = buyopen*buyEntryPrice;
		double tmpPretotalVolum = buyopen;
        buyopen -= er.getTradeVol();

        if(buyopen == 0){
            buyEntryPrice = 0;
        }
        else{
            //buyAvg = (tmp1 - tmp2)/buyopen;
			buyEntryPrice = (tmpPretotalPrice-er.getTradePrice()*er.getTradeVol())/buyopen;
        }	
    }

    if(er.getType() == Stype::Sellshort){
        if(sellopen == 0){
            sellEntryPrice = er.getTradePrice();
        }else
		{
			sellEntryPrice = (sellEntryPrice*sellopen+er.getTradePrice()*er.getTradeVol())/(sellopen+er.getTradeVol());
		}
        sellopen += er.getTradeVol();            
    }

    if(er.getType() == Stype::Buytocover){    
		double tmpPretotalPrice = sellopen*sellEntryPrice;
        sellopen -= er.getTradeVol();
            //end
        if(sellopen == 0){
            sellEntryPrice = 0;
        }
        else{
            //sellAvg = (tmp1 - tmp2)/sellopen;
			sellEntryPrice = (tmpPretotalPrice-er.getTradePrice()*er.getTradeVol())/sellopen;
        }
    }
}
