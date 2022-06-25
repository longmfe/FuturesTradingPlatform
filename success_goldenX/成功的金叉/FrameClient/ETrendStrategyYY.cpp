#include "stdafx.h"
#include "ETrendStrategyYY.h"
#include <string>
#include <list>
#include "TickData.h"
#include "CloseIndexUnit.h"
#include "rwArgsFile.h"
#include "GlobalDataManage.h"
#include "UtilFun.h"
#include "rwIndexFile.h"
#include "Order.h";
#include "QRCadapter.h";
using namespace std;
extern GlobalDataManage* pGlobalDm;
extern AlgoTSEng* ats;
CloseIndexUnit* close1;
CloseIndexUnit* close2;
bool bwrite;
//分别开仓的时间，已BAR来计算
int buyBar;
double buygap;
int sellBar;
double sellgap;
bool g_bInNewMonth = true;;

//added by aaron 2013-2-5
QRCadapter* qrc;


TIMERPROC   FooTimerFun(   HWND,   UINT,   UINT,   DWORD   ) 
{ 
	UtilFun uf;
	
	string ss = uf.getCHCurDate().substr(8, 4);
	
	int hhmm = atoi(ss.c_str());
                
    if(bwrite && hhmm > 1520){
        rwIndexFile rif;
        list<double> tmp1;
        list<double> tmp2;
        if(!g_bInNewMonth){
			close1->clear();
			close2->clear();
			rif.writeIndexFile(tmp1, tmp2);
			rwArgsFile  raf;
			raf.writeArgsFile(-10000, 0, -10000, 0);
			bwrite = false;
			return 0;
		}
        int len = close1->getSize();
        //if(len > 300){
        //    len = 300;
        //}
        for(int i = len - 1; i >= 0; i --){
			tmp1.push_back(close1->getIndex(-1 - i));
            tmp2.push_back(close2->getIndex(-1 - i));
        }
                   
                    
        rif.writeIndexFile(tmp1, tmp2);
                    
        rwArgsFile  raf;
        raf.writeArgsFile(buyBar, buygap, sellBar, sellgap);
        bwrite = false;
    }
                
    if(!bwrite && hhmm < 1515){
        bwrite = true;
    }

	return 0;
} 

//返回周几
int CaculateWeekDay(int y,int m, int d)
{
	if(m==1||m==2) {
		m+=12;
		y--;
	}
	return (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;	
} 

//返回第几周
int CaculateWeekOfMonth(int y,int m, int d)
{
	int res = 0;
	int tmp = CaculateWeekDay(y, m, 1);
	if( tmp > 4){
		res = (d - (7 - tmp) - 1)/7 ; 
	}
	else{
		res = (d + tmp - 1)/7 ; 
	}

	return res;
} 

int ETrendStrategyYY::getCurrentBar(){
	return close1->getSize() > close2->getSize()?close1->getSize():close2->getSize();
}


ETrendStrategyYY::ETrendStrategyYY(int min, string sec1, string sec2, int lots){
	strategyID = "ETrendStrategyYY";
	Sec1 = sec1;
	Sec2 = sec2;
	Min = min;
	lot = lots;
	

	date = "xx";

	initValue = -10000;

	//open positions's bar; record the number of the indexunit
    currentbar = -1;
    lastbar = 1;
    indexb = -1;

    bInTrade = false;//进入可交易区间
    bInTradeLastDay = false;//进入可交易区间的最后一天
    bInNewMonth = false;//已换约
    bGap = true;//gap is ok
    
    //分别开仓的时间，已BAR来计算
    buyBar = initValue;
    buygap = 0;
    sellBar = initValue;
    sellgap = 0;
    
    
    boolean bwrite = true;
    
    //for cancel
    checkFre = 15;
    checkbegintime1 = 0;
    checkbegintime2 = 0;
    bCheckCancel1 = false;
    bCheckCancel2 = false;
    
    bCon11 = false;
    bCon21 = false;
    bCon31 = false;
    
    bCon12 = false;
    bCon22 = false;
    bCon32 = false;

	checkOr1 = nullptr;
    checkOr2 = nullptr;
    checkOr3 = nullptr;
    checkOr4 = nullptr;


	init();
}


void ETrendStrategyYY::initForStart(){
	rwArgsFile raf;
	raf.readArgsFile();

	buyBar = raf.getBuyBar();
    sellBar = raf.getSellBar();

    buygap = raf.getBuyGap();
    sellgap = raf.getSellGap();


	rwIndexFile rif;
    rif.readIndexFile();
    list<double> tmp1 = rif.getIndexList1();
	for(list<double>::iterator it = tmp1.begin(); it !=  tmp1.end(); it ++){
		//char tmp[20] = {0};
		//sprintf(tmp,"%lf",*it);
        IndexDetailUnit iu(string(""),string(""),0,*it);
        close1->addIndexDetails(iu);
    }

    list<double> tmp2 = rif.getIndexList2();
    for(list<double>::iterator it = tmp2.begin(); it !=  tmp2.end(); it ++){
        //char tmp[20] = {0};
		//sprintf(tmp,"%lf",*it);
        IndexDetailUnit iu(string(""),string(""),0,*it);
        close2->addIndexDetails(iu);
        
    }

	list<double>::iterator it2 = tmp2.begin();
	for(list<double>::iterator it1 = tmp1.begin(); it1 !=  tmp1.end(); it1 ++, it2++){
		gapL.push_back(*it2 - *it1);        
    }	
    indexb = tmp2.size();
}


void ETrendStrategyYY::InitDD(TickData ss){

	bInNewMonth = true;
    bInTrade = true;
    bInTradeLastDay = false;

	int idate = atoi(ss.getDate().c_str());

	//the 3rd week
	if(CaculateWeekOfMonth(idate/10000, (idate%10000/100), idate%100) == 2){
		int day = CaculateWeekDay(idate/10000, (idate%10000/100), idate%100);
		if(4 == day){
			//zhouwu
			bInTrade = false;
            bInNewMonth = false;
		}
		else if(3 == day){
			//zhousi
			bInTradeLastDay = true;
		}
	}
	g_bInNewMonth = bInNewMonth;
	pGlobalDm->keepRecord(persnal1, "Today is " + ss.getDate());
}

void ETrendStrategyYY::init(){
	string str;
	char tmp[20] = {0};
	itoa(Min,tmp,10);
	close1 = (CloseIndexUnit*) pGlobalDm->getIndexMana()->getIndexUnit("Close" + string(tmp) + Sec1);
    close2 = (CloseIndexUnit*) pGlobalDm->getIndexMana()->getIndexUnit("Close" + string(tmp) + Sec2);

    SetTimer(   NULL, 10000, 1000*60*2 ,   (TIMERPROC)FooTimerFun ); 
    initForStart();
	qrc = new QRCadapter();
	qrc->connect();

}

void ETrendStrategyYY::run(){
	TickData ss = *(pGlobalDm->getMdMana()->getTickData());
	qrc->MDFeedin(ss);
     //cout<<ss.getTime();

	

	if(strncmp(Sec1.c_str(), ss.getSec().c_str(), Sec1.length()) == 0){
		ss1 = ss;
	}
	else if(strncmp(Sec2.c_str(), ss.getSec().c_str(), Sec2.length()) == 0){
		ss2 = ss;
	}
	else{
		return;
	}

	//for cancel
    if(bCheckCancel1 && checkOr1 != nullptr && checkOr2 != nullptr){
        doCancel(ss);
    }
        
    if(bCheckCancel2 && checkOr3 != nullptr && checkOr4 != nullptr){
        doCancel_2(ss);
    }

	//换日initdd
	if(strcmp(date.c_str(), ss.getDate().c_str()) != 0){
        InitDD(ss);
        date = ss.getDate();     
    }  
	
    if(!bInNewMonth){
        gapL.clear();
        return;
    }

	//new bar --->in
    currentbar = getCurrentBar();
    if(bInNewMonth && indexb != currentbar){
        //修改之，避免0指标的出现 
        if(close2->getIndex(-2) != 0 && close1->getIndex(-2) != 0 && close1->getSize() == close2->getSize()){
            double tmp = close2->getIndex(-2) - close1->getIndex(-2);
			gapL.push_back(tmp); 
            indexb = currentbar;
            lastbar = currentbar;
        }  
        else if(close2->getIndex(-1) != 0 && close1->getIndex(-2) != 0 && close1->getSize() > close2->getSize()){
            double tmp = close2->getIndex(-1) - close1->getIndex(-2);
            gapL.push_back(tmp); 
            indexb = currentbar;
            lastbar = currentbar;
        }
        else{
            return;
        }
    }

    //最后1分钟特殊处理
	if((!bInTradeLastDay && strcmp(ss.getTime().substr(0, 5).c_str(), "15143") > 0) 
		|| (strcmp(ss.getTime().substr(0, 5).c_str(), "11293") > 0 && strcmp(ss.getTime().substr(0, 5).c_str(),"11295") < 0)){
        list<double> tmpl;
        if(gapL.size() >= 300){
            bInTrade = true;
            //jiequ排序
            
			int counts = gapL.size();
			int i = 0;
			for(list<double>::iterator it = gapL.begin(); it != gapL.end(); it ++, i ++){
				if(i >= counts - 299){
					tmpl.push_back(*it);
				}
            }
            double tmp = close2->getIndex(-1) - close1->getIndex(-1);
			tmpl.push_back(tmp);            
			tmpl.sort();

            //low15 = tmpl.get(14);
			i = 0;
			for(list<double>::iterator it = tmpl.begin(); it != tmpl.end(); it ++, i ++){
				if(i == 14){
					low15 = *it;
				}
				else if(i == 285){
					high15 = *it;
				}
            }
        }
        if(gapL.size() < 300){
            return;
        }
            
        if(bInTrade){
            //实时变化
            double gap = close2->getIndex(-1) - close1->getIndex(-1);
            //优先平仓
            if(gap - buygap >= 3 && buyBar != initValue){
                //close position                    
                Order*  or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 2,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
				ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() - 2,Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or);
                buyBar = initValue;
            }
                
            if(gap - sellgap <= -3 && sellBar != initValue){
                //close position
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 2, Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() + 2,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or1);
                sellBar = initValue;                    
            }
                
            if(buyBar != initValue && currentbar - buyBar >= 300 && gap >= high15){
                    
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 2,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() - 2,Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or1);
                buyBar = initValue;
            }
                
            if(sellBar != initValue && currentbar - sellBar >= 300 && gap <= low15){
                sellBar = initValue;
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 2,Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() + 2,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or1);
            }
                
               
                
            if(gap >= high15 && sellBar == initValue){
                    
                //判断是否已有仓位，如无则开仓
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 2,Buy, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() - 2,Sellshort, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or1);
                sellBar = currentbar;
                sellgap = gap;
            }
                
            if(gap <= low15 && buyBar ==  initValue){
                    
                //判断是否已有仓位，如无则开仓
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 2,  Sellshort, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
                //stl.sendOrder(or);
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice() + 2,Buy, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
                //stl.sendOrder(or1);
                buyBar = currentbar;
                buygap = gap;
            }
        }            
    }
        
	if(lastbar == currentbar && strcmp(ss.getTime().c_str(),"0916") > 0){
        lastbar++;            
        
		list<double> tmpl;
        if(gapL.size() >= 300){
            bInTrade = true;
            //jiequ排序            
			int counts = gapL.size();
			int i = 0;
			for(list<double>::iterator it = gapL.begin(); it != gapL.end(); it ++, i ++){
				if(i >= counts - 300){
					tmpl.push_back(*it);
				}
            }
                      
			tmpl.sort();
			i = 0;
			for(list<double>::iterator it = tmpl.begin(); it != tmpl.end(); it ++, i ++){
				if(i == 14){
					low15 = *it;
				}
				else if(i == 285){
					high15 = *it;
				}
            }
        }
        if(gapL.size() < 300){
            return;
        }


        if(bInTrade){

			double gap = gapL.back();
            //优先平仓
			if(bInTradeLastDay && strcmp(ss.getTime().c_str(),"151000") > 0){
                
                if(sellBar > initValue){
                    Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                            Sec1, ss1.getLastPrice() - 0.4,Sell, 0.4, lot, ss.getDate(), ss.getTime());
                    ats->cc->sendorder(or);
					pGlobalDm->keepRecord(HisOrder, or->getString());
					//stl.sendOrder(or);
                    checkOr1 = or;
                    Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                            Sec2, ss2.getLastPrice(),Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                    ats->cc->sendorder(or1);
					pGlobalDm->keepRecord(HisOrder, or1->getString());
					//stl.sendOrder(or1);
                    checkOr2 = or1;
                    bCheckCancel1 = true;
                }
                    
                if(buyBar > initValue){
                    Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                            Sec1, ss1.getLastPrice() + 0.4,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                    ats->cc->sendorder(or);
					pGlobalDm->keepRecord(HisOrder, or->getString());
					//stl.sendOrder(or);
                    checkOr3 = or;
                    Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                            Sec2, ss2.getLastPrice(),Sell, 0.4, lot, ss.getDate(), ss.getTime());
                    ats->cc->sendorder(or1);
					pGlobalDm->keepRecord(HisOrder, or1->getString());
					//stl.sendOrder(or1);
                    checkOr4 = or1;
                    bCheckCancel2 = true;
                }
                sellBar = initValue;
                buyBar = initValue;
                return;
            }
                
            //后平正常交易日的                
            if(gap - buygap >= 3 && buyBar != initValue){
                //close position                    
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 0.4,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr1 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(),Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or1);
                buyBar = initValue;
                //for cancel
                checkOr2 = or1;
                bCheckCancel1 = true;
            }
                
            if(gap - sellgap <= -3 && sellBar != initValue){
                //close position
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 0.4,Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr1 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(),Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or);
                sellBar = initValue;
                //for cancel
                checkOr2 = or1;
                bCheckCancel1 = true;
            }
                
            if(buyBar != initValue && currentbar - buyBar >= 300 && gap >= high15){
                    
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 0.4,Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr1 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(), Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or1);
                buyBar = initValue;
                //for cancel
                checkOr2 = or1;
                bCheckCancel1 = true;
            }
                
            if(sellBar!= initValue && currentbar - sellBar >= 300 && gap <= low15){
                sellBar = initValue;
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 0.4, Sell, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr1 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(), Buytocover, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or1);
                //for cancel
                checkOr2 = or1;
                bCheckCancel1 = true;
            }
                
            //开仓
			if(bInTradeLastDay && strcmp(ss.getTime().c_str(),"093500") < 0){
                //最后1个交易日早于20分钟不开仓
                return;
            }
                
                
                
            if(gap >= high15 && sellBar == initValue){
                    
                //判断是否已有仓位，如无则开仓
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() + 0.4,Buy, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr3 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(), Sellshort, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or1);
                sellBar = currentbar;
                sellgap = gap;
                //for cancel
                checkOr4 = or1;
                bCheckCancel2 = true;
            }
                
            if(gap <= low15 && buyBar == initValue){
                    
                //判断是否已有仓位，如无则开仓
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec1, ss1.getLastPrice() - 0.4, Sellshort, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
				pGlobalDm->keepRecord(HisOrder, or->getString());
				//stl.sendOrder(or);
                //for cancel
                checkOr3 = or;
                Order* or1 = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    Sec2, ss2.getLastPrice(),Buy, 0.4, lot, ss.getDate(), ss.getTime());
                ats->cc->sendorder(or1);
				pGlobalDm->keepRecord(HisOrder, or1->getString());
				//stl.sendOrder(or1);
                buyBar = currentbar;
                buygap = gap;
                //for cancel
                checkOr4 = or1;
                bCheckCancel2 = true;
            }
        }
    }

}


 void ETrendStrategyYY::doCancel(TickData ss){
	 int time = ((int)atof(ss.getTime().c_str()))%100;
    //check time  and times  独立计算
    int times = time/checkFre;
        
        
    if(times > 0 && checkbegintime1 != times){ 
        checkbegintime1 = times;
        //check orderstatus and do sth.
        //all done
        if(checkOr1->getStatus() == Order::Filled && checkOr2->getStatus() == Order::Filled ){
            bCheckCancel1 = false;
            checkOr1 = nullptr;
            checkOr2 = nullptr;
            bCon11 = false;
            bCon21 = false;
            bCon31 = false;
            checkbegintime1 = 0;
            return;
        }
                       
            
        //contion2              
        if(checkOr1->getStatus() == Order::Filled && checkOr2->getStatus() <= Order::PartialFill ){
            //check time
            ats->cc->cancelOrder(checkOr2);     
            pGlobalDm->keepRecord(HisOrder, checkOr2->getString() + "--Cancell Order");
            //System.out.println("=================cancelOrder(checkOr2)" + checkOr2.getString());
            bCon21 = true;
        }    
                      
    }
        
        
    //追单
	if(checkOr2->getStatus() == Order::Cancelled && bCon21 && strcmp(checkOr2->getSec().c_str(), ss.getSec().c_str()) == 0){
        Order* or = nullptr;
        int unitPrice = 1;
        if(checkOr2->getType() == Sellshort  || checkOr2->getType() == Sell){
            unitPrice = -1;
        }
        switch(times){
        case 1:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr2->getSec(), ss.getLastPrice() + unitPrice, checkOr2->getType(), 
                    0.4, checkOr2->getSendVol(), ss.getDate(), ss.getTime());
            break;
        case 2:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr2->getSec(), ss.getLastPrice() + 2 * unitPrice, checkOr2->getType(), 
                    0.4, checkOr2->getSendVol(), ss.getDate(), ss.getTime());
            break;
        case 3:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr2->getSec(), ss.getLastPrice() + 20 * unitPrice, checkOr2->getType(), 
                    0.4, checkOr2->getSendVol(), ss.getDate(), ss.getTime());
            break;
        }
            
        ats->cc->sendorder(or);
        pGlobalDm->keepRecord(HisOrder, or->getString() + "--ReSend orders");
        checkOr2 = or;
        bCon21 = false;
        //System.out.println(ss.getLastPrice() + "=================checkOr2.getStatus() == Order.Cancelled && bCon2" + or.getString());
    }
        
    if(checkOr1->getStatus() == Order::Filled && checkOr2->getStatus() == Order::Cancelled && bCon31){
        checkOr1 = nullptr;
        checkOr2 = nullptr;
        bCon31 = false;
        bCheckCancel1 = false;
        checkbegintime1 = 0;
        //System.out.println(ss.getLastPrice() + "checkOr1.getStatus() == Order.Filled && checkOr2.getStatus() == Order.Cancelled && bCon3");           
    }
        
    if(times > 2){
        bCheckCancel1 = false;
        checkOr1 = nullptr;
        checkOr2 = nullptr;
        bCon11 = false;
        bCon21 = false;
        bCon31 = false;
        checkbegintime1 = 0;
    }
}
  
void ETrendStrategyYY::doCancel_2(TickData ss){
	int time = ((int)atof(ss.getTime().c_str()))%100;
    //check time  and times  独立计算
    int times = time/checkFre;
    double gaplocal = ss2.getLastPrice() - ss1.getLastPrice();
    //这边都是主要处理买方向的
    if(times > 0 && checkbegintime2 != times){ 
        checkbegintime2 = times;
        //check orderstatus and do sth.
        //all done
        if(checkOr3->getStatus() == Order::Filled && checkOr4->getStatus() == Order::Filled ){
            bCheckCancel2 = false;
            checkOr3 = nullptr;
            checkOr4 = nullptr;
            bCon12 = false;
            bCon22 = false;
            bCon32 = false;
            checkbegintime2 = 0;
            return;
        }
            
            
        //test condition
        if(checkOr3->getType() == Buy){
            if(gaplocal <= low15 || sellgap -3 >= gaplocal){
                bCon32 = true;
            }
            else{
                bCon22 = true;
            }
        }
            
        if(checkOr3->getType() == Sellshort){
            if(gaplocal >= high15 || buygap + 3 <= gaplocal){
                bCon32 = true;
            }
            else{
                bCon22 = true;
            }
        }


        //contion3
        if(checkOr3->getStatus() == Order::Filled && checkOr4->getStatus() <= Order::PartialFill && bCon32){
            //check time                
            //close position                                
            if(checkOr3->getType() == Buy || checkOr3->getType() == Sell){
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    checkOr3->getSec(), ss1.getLastPrice() - 0.4, Sell, 
                                    0.4, checkOr3->getSendVol(), ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
                pGlobalDm->keepRecord(HisOrder, or->getString() + "--Close the single leg1");
                checkOr3 = or;
                //System.out.println("=================closePosition(checkOr3)" + checkOr3.getString());
                sellBar = initValue;
            }
            else if(checkOr3->getType() == Sellshort || checkOr3->getType() == Buytocover){
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                        checkOr3->getSec(), ss1.getLastPrice() + 0.4,Buytocover, 
                        0.4, checkOr3->getSendVol(), ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
                pGlobalDm->keepRecord(HisOrder, or->getString() + "--Close the single leg2");
                checkOr3 = or;   
                //System.out.println("=================closePosition(checkOr3)" + checkOr3.getString());
                buyBar = initValue;
            }
                
            ats->cc->cancelOrder(checkOr4);  
            pGlobalDm->keepRecord(HisOrder, checkOr4->getString() + "--Cancell Order1");
        }   
            
        if(checkOr3->getStatus() < Order::Filled && checkOr4->getStatus() == Order::Cancelled && bCon32){
            //check time
            bCon32 = true;
            //close position        
            ats->cc->cancelOrder(checkOr3); 
            if(checkOr3->getType() == Buy || checkOr3->getType() == Sell){
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                                    checkOr3->getSec(), ss1.getLastPrice() - 0.4,Sell, 
                                    0.4, checkOr3->getSendVol(), ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
                pGlobalDm->keepRecord(HisOrder, or->getString() + "--Close the single leg3");
                checkOr3 = or;
                //System.out.println("222=================closePosition(checkOr3)" + checkOr3.getString());
            }
            else if(checkOr3->getType() == Sellshort|| checkOr3->getType() == Buytocover){
                Order* or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                        checkOr3->getSec(), ss1.getLastPrice() + 0.4,Buytocover, 
                        0.4, checkOr3->getSendVol(), ss.getDate(), ss.getTime());
                ats->cc->sendorder(or);
                pGlobalDm->keepRecord(HisOrder, or->getString() + "--Close the single leg4");
                checkOr3 = or;   
                //System.out.println("222=================closePosition(checkOr3)" + checkOr3.getString());
            }
                
        }   
            
        //contion2
        if(checkOr3->getStatus() == Order::Filled && checkOr4->getStatus() <= Order::PartialFill && bCon22){
            //check time
            ats->cc->cancelOrder(checkOr4);     
            pGlobalDm->keepRecord(HisOrder, checkOr4->getString() + "--Cancell Order2");
            //System.out.println("=================cancelOrder(checkOr2)" + checkOr4.getString());
            bCon22 = true;
        }      
            
        if(times > -1000){
            return;
        }
                      
    }
        
        
    //追单
    if(checkOr4->getStatus() == Order::Cancelled && bCon22 && strcmp(checkOr4->getSec().c_str(),ss.getSec().c_str()) == 0){
        Order* or = nullptr;
        int unitPrice = 1;
        if(checkOr4->getType() == Sellshort || checkOr4->getType() == Sell){
            unitPrice = -1;
        }
        switch(times){
        case 1:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr4->getSec(), ss.getLastPrice() + unitPrice, checkOr4->getType(), 
                    0.4, checkOr4->getSendVol(), ss.getDate(), ss.getTime());
            break;
        case 2:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr4->getSec(), ss.getLastPrice() + 2 * unitPrice, checkOr4->getType(), 
                    0.4, checkOr4->getSendVol(), ss.getDate(), ss.getTime());
            break;
        case 3:
            or = new Order(strategyID, pGlobalDm->getOrdersMana()->getLastOrderID(), 
                    checkOr4->getSec(), ss.getLastPrice() + 20 * unitPrice, checkOr4->getType(), 
                    0.4, checkOr4->getSendVol(), ss.getDate(), ss.getTime());
            break;
        }
            
        ats->cc->sendorder(or);
        pGlobalDm->keepRecord(HisOrder, or->getString() + "--ReSend orders");
        checkOr4 = or;
        bCon22 = false;
        //System.out.println(ss.getLastPrice() + "=================checkOr4.getStatus() == Order.Cancelled && bCon2" + or.getString());
    }
        
    if(checkOr3->getStatus() == Order::Filled && checkOr4->getStatus() == Order::Cancelled && bCon32){
        checkOr3 = nullptr;
        checkOr4 = nullptr;
        bCon32 = false;
        bCheckCancel2 = false;
        checkbegintime2 = 0;
        //System.out.println(ss.getLastPrice() + "checkOr3.getStatus() == Order.Filled && checkOr4.getStatus() == Order.Cancelled && bCon3");           
    }
        
    if(times > 2){
        bCheckCancel2 = false;
        checkOr3 = nullptr;
        checkOr4 = nullptr;
        bCon12 = false;
        bCon22 = false;
        bCon32 = false;
        checkbegintime2 = 0;
    }
}
  