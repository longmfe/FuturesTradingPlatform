#include "StdAfx.h"
#include "DealOrdersLocal.h"
#include "TickData.h"
#include "Order.h"
#include "UtilFun.h"
  

DealOrdersLocal::DealOrdersLocal(GlobalDataManage*  ma){
	m_ma = ma;
}

DealOrdersLocal::~DealOrdersLocal(){
}

void DealOrdersLocal::init(){
}

void DealOrdersLocal::run(){
	TickData sec = *(m_ma->getMdMana()->getTickData());

	if(m_ma->getOrdersMana()->getUnTradeOrder().size() < 1){
		return;
	}

	list<Order*> tmplist = m_ma->getOrdersMana()->getUnTradeOrder();
	list<Order*>::iterator it = tmplist.begin();
	for(; it != tmplist.end(); it++){
		if(strncmp(sec.getSec().c_str(), (*it)->getSec().c_str(),(*it)->getSec().length()) == 0){
			//支持市价单 aaron 2013-3-26
			if( 0 == (*it)->getSendPrice()){
			 //marketOrder-Buy open
				if((*it)->getType() == Buy)
				{
					int want = ((*it)->getSendVol()) - ((*it)->getTradeVolume());
					
						(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

					(*it)->TradeOccur(sec.getLastPrice(),want);
					m_ma->getPositionMana()->addOrder(*it);
					m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);	
				}
			 //marketOrder-Sell close
				else if((*it)->getType() == Sell)
				{
				    int want = ((*it)->getSendVol()) - ((*it)->getTradeVolume());
					PositionManage* pm = m_ma->getPositionMana();
					if(pm->getPositionBysec(sec.getSec())==NULL || pm->getPositionBysec(sec.getSec())->getBuyopen()<want)
					{
					   continue;
					} 
					
					(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

				    (*it)->TradeOccur(sec.getLastPrice(),want);
					m_ma->getPositionMana()->addOrder(*it);
					m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
				}
			 //marketOrder-Sellshort
				else if((*it)->getType() == Sellshort)
				{
				    int want = ((*it)->getSendVol()) - ((*it)->getTradeVolume());
					
					(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());
					(*it)->TradeOccur(sec.getLastPrice(),want);
					m_ma->getPositionMana()->addOrder(*it);
					m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);	
				}
		     //marketOrder-Buytocover
				else if((*it)->getType() == Buytocover)
				{
					int want = ((*it)->getSendVol()) - ((*it)->getTradeVolume());
					PositionManage* pm = m_ma->getPositionMana();
					if(pm->getPositionBysec(sec.getSec())==NULL || pm->getPositionBysec(sec.getSec())->getSellopen()<want)
					{
					   continue;
					} 
					
					(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

				    (*it)->TradeOccur(sec.getLastPrice(),want);
					m_ma->getPositionMana()->addOrder(*it);
					m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
				}
			}
		
			double slipprice = (*it)->getSlip();

			if((*it)->getType() == Buy && sec.getAskPrice() <= (*it)->getSendPrice() + slipprice){
				int want = (*it)->getSendVol() - (*it)->getTradeVolume();
				int real = (int)sec.getAskVol();
				if(real >= 2*want){
					if(want != 0){
						
							(*it)->setTradeTime((*it)->getSendTime()+ "tradetime:" + sec.getTime() + "Date:" + sec.getDate());

					}
					(*it)->TradeOccur(sec.getAskPrice(),want);
					m_ma->getPositionMana()->addOrder(*it);
					//pGlobalDm->getOrdersMana()->getUnTradeOrder().remove(*it);
					m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
				}
				else{
					if((int)real/2 != 0){
						
						(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());


						(*it)->TradeOccur(sec.getAskPrice(),(int)real/2);
						m_ma->getPositionMana()->addOrder(*it);
					}
				}
			}
			else if((*it)->getType() == Buytocover && sec.getAskPrice() <= (*it)->getSendPrice() + slipprice){
                int want = (*it)->getSendVol() - (*it)->getTradeVolume();
                int real = (int)sec.getAskVol();
                                        
                PositionManage* pm = m_ma->getPositionMana();
				int index = sec.getSec().find(" ");
				string name;
				if(index < 0){
					name = sec.getSec();
				}
				else{
					name = sec.getSec().substr(0, index);
				}

                if(pm->getPositionBysec(name)->getSellopen() < want){
                    continue;
                }
                                    
                if(real >= 2 * want){                                               
                    //test or realEnv                                            
                    if(want != 0){
                        
                        (*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());
                        (*it)->TradeOccur(sec.getAskPrice(), want);
                        m_ma->getPositionMana()->addOrder(*it);   
                        m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
                    }
                }
                else{
                    //test or realEnv                                            
                    if((int)real/2 != 0){
                       (*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

                        (*it)->TradeOccur(sec.getAskPrice(), (int)real/2);
                        m_ma->getPositionMana()->addOrder(*it);   
                        //AlgoTSEng.GlobalDm.getOrdersMana().getSendingOrder().remove(i);
                    }
                }
			}
			//卖开
			else if((*it)->getType() == Sellshort &&sec.getBidPrice() >= (*it)->getSendPrice() - slipprice){
                                       
                int want = (*it)->getSendVol() - (*it)->getTradeVolume();
                int real = (int)sec.getBidVol();
                if(real >= 2 * want){   
                    //test or realEnv                                            
                    if(want != 0){
                                                
                     
                       (*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

                        (*it)->TradeOccur(sec.getBidPrice(), want);
                        m_ma->getPositionMana()->addOrder(*it);   
                        m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
                    }
                }
                else{                                            
                                           
                    if((int)real/2 != 0){
                        
                        (*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

                        (*it)->TradeOccur(sec.getBidPrice(), (int)real/2);
                        m_ma->getPositionMana()->addOrder(*it);   
                        //AlgoTSEng.GlobalDm.getOrdersMana().getSendingOrder().remove(i);
                    }
                }
			}
             //卖平
			else if((*it)->getType() == Sell && sec.getBidPrice() >= (*it)->getSendPrice() - slipprice){
                                    
                int want = (*it)->getSendVol() - (*it)->getTradeVolume();
                int real = (int)sec.getAskVol();
                PositionManage* pm = m_ma->getPositionMana();
				int index = sec.getSec().find(" ");
				string name;
				if(index < 0){
					name = sec.getSec();
				}
				else{
					name = sec.getSec().substr(0, index);
				}
                if(pm->getPositionBysec(name)->getBuyopen() < want){
                    continue;
                }                                    
                                        
                if(real >= 2 * want){        
                    if(want != 0){
                        
							(*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

                        (*it)->TradeOccur(sec.getBidPrice(), want);
                        m_ma->getPositionMana()->addOrder(*it);  
                        m_ma->getOrdersMana()->RemoveUnTradeOrder(*it);
                    }
                }
                else{                                            
                                            
                    if((int)real/2 != 0){ 
                        //test or realEnv            
                        (*it)->setTradeTime(sec.getDate()+ "," + (*it)->getSendTime()+ "," + sec.getTime());

                        (*it)->TradeOccur(sec.getBidPrice(), (int)real/2);
                        m_ma->getPositionMana()->addOrder(*it);   
                        //AlgoTSEng.GlobalDm.getOrdersMana().getSendingOrder().remove(i);
                    }
                }
			}
		}
	}
}