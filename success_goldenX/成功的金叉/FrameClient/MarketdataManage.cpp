#include "StdAfx.h"
#include "MarketdataManage.h"
 
MarketdataManage::MarketdataManage(GlobalDataManage* ma)
{
	m_ma = ma;
}


MarketdataManage::~MarketdataManage(void)
{
	list<TickData*>::iterator it=  secList.begin();
	for(;it!=secList.end();it++)
	{
		if(*it!=NULL)
		{
			delete *it;
			*it= NULL;
		}
	
	}
	map<string, list<TickData*>>::iterator mit =secMap.begin();
	for(;mit!=secMap.end();mit++)
	{
		list<TickData*>::iterator itt = mit->second.begin();
		for(;itt!=mit->second.end();itt++)
		{
			if(*itt!=NULL)
			{
				delete *itt;
				*itt = NULL;
			}
			
		}
	}
}
void MarketdataManage::setDefaultSec(string con){
            firstCon = con;
}
TickData* MarketdataManage::getTickDataByIndex(int index)
{
	list<TickData*>::iterator it=  secList.begin();
	if(index >secList.size())
	{
		return NULL;
	}else
	{
		int i =0;
		while(i<index)
		{
			i++;
			it++;
		}
		return *it;
	}
}
 TickData* MarketdataManage::getTickDataByIndex(string con, int index)
 {
        if(con == firstCon)
		{
            if(index >= secList.size())
			{
               return NULL;
			}

			list<TickData*>::iterator it=  secList.begin();
            int i =0;

			while(i<index)
			{
				i++;
				it++;
			}
			return *it;
        }
        else if(secMap.find(con)!=secMap.end()){
			if(index >= secMap.find(con)->second.size()){
				return NULL;
           }
           list<TickData*>::iterator it =secMap.find(con)->second.begin();
		   int i =0;

			while(i<index)
			{
				i++;
				it++;
			}
			return *it;
        }
        else{
              
			return NULL;
        }
}
void  MarketdataManage::addTickData(TickData* sec)
{

	if(secList.size() > 600)  //35000
	{
		list<TickData*>::iterator it11 = secList.begin();
		for(;it11!=secList.end();it11++)
		{
			if(*it11!=NULL)
			{
				delete *it11;
				*it11 =NULL;
			}
		}
		secList.clear();
	}
	secList.push_back(sec);
	//pGlobalDm.getPositionMana().getPositionBysec(sec.getSec());
	 
	Position* tmp =m_ma->getPositionMana()->getPositionBysec(sec->getSec());
	if(tmp ==NULL)
	{
		return;
	}else
	{
		tmp->computeProfit(sec->getLastPrice());
	}	
}
 void  MarketdataManage::addTickDatas(TickData* sec)
 {
		
	//string tmpstr =sec->sec;
	 toolsClass tc ;
	 string tmpSecstr = tc.getPreStrBySplit(sec->getSec()," ");

	if(strcmp(tmpSecstr.c_str(),firstCon.c_str())==0){
		secList.push_back(sec);
			if(secList.size() > 35000){
                secList.clear();
            }
    }
    else if(secMap.find(tmpSecstr)!=secMap.end()){
		if(secMap.find(tmpSecstr)->second.size() > 35000){
                secMap.find(tmpSecstr)->second.clear();
            }
		secMap.find(tmpSecstr)->second.push_back(sec);
    }
    else{
            list<TickData*> tmp;	
			tmp.push_back(sec);
                
            secMap.insert(pair<string, list<TickData*>>(tmpSecstr, tmp));
    }

    //compute position's floatProfit
    Position* tmp = m_ma->getPositionMana()->getPositionBysec(tmpSecstr);
    if(tmp == NULL)
	{
		return;
	}else
	{
		tmp->computeProfit(sec->getLastPrice());
	}	

}
int MarketdataManage::getLen()
{
        return secList.size();
}
 int MarketdataManage::getLen( string con)
 {
	 if(strcmp(con.c_str(),firstCon.c_str())==0){
            return secList.size();
        }
        else if(secMap.find(con)!=secMap.end()){
			return secMap.find(con)->second.size();
        }
        else{
            return 0;
        }
 }
TickData* MarketdataManage::getTickData()
{
	return secList.back();
}
TickData* MarketdataManage::getTickData(string con)
{

	 if(strcmp(con.c_str(),firstCon.c_str())==0)
	 {
		 return secList.back();
     }
     else if(secMap.find(con)!=secMap.end()){
          
		 return secMap.find(con)->second.back();
     }
     else{
		return NULL;
     }
}