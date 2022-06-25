#include "StdAfx.h"
#include "CloseIndexUnit.h"
#include "DoubleIndexValue.h"

CloseIndexUnit::CloseIndexUnit(void)
{
		name = "Close";
		exchangeName = "Not_CFFEX";
		date =  "default";
		lastMin = 0;
		lastBarNum = 0;
		len = 0;
		close = 0.0;
		inValidV = 0;
		toolsClass tc;
		//name += "" + tc.IntToString(min) + sec;
		this->mode = 1;
}
CloseIndexUnit::~CloseIndexUnit(void)
{
	
}

CloseIndexUnit::CloseIndexUnit(int min, int initHHMM, string sec){
		name = "Close";
		exchangeName = "Not_CFFEX";
		date =  "default";
		lastMin = 0;
		lastBarNum = 0;
		len = 0;
		close = 0.0;
		inValidV = 0;
		toolsClass tc;
		name += "" + tc.IntToString(min) + sec;
		 
		iInitHHMM = initHHMM;
		this->min = min;
		this->Sec = sec;
		if(915 == initHHMM){
		exchangeName = "CFFEX";
		}
		this->barSeq = this->GetBarSeq(this->iInitHHMM*100, this->min, this->exchangeName);
		////0-using in server side ; 1- using in client side
		this->mode = 1;

}
	
int CloseIndexUnit::timeToHourmin(string time) {
	int result = 0;
	
	toolsClass tc;

	 
	string tmp = tc.deleteSubStr(time,':');
	result = tc.stringToInt(tmp);
	string tt;
	tmp = "" + tc.IntToString(result);
	tt = tmp.substr(0, 1);
	result = tc.stringToInt(tt);
	if (result > 1) {
		tt = tmp.substr(0, 3);
		result = tc.stringToInt(tt);
	} else {
		tt = tmp.substr(0, 4);
		result = tc.stringToInt(tt);
	}
	return result;
};
	
list<int> CloseIndexUnit::GetBarSeq(int InitTime,int cycle,string exName) 
{
	    list<int> barseq ;
    	
    	int curBar = InitTime;
		if(strcmp(exName.c_str(),"CFFEX")==0)
    	{
    		if(cycle>15)
    		{
    			if((InitTime%10000)/100!= 0)
    			{
    				curBar = InitTime - ((InitTime%10000)/100)*100;
    			}
    		}
    	}
		barseq.push_back(InitTime);
    	while(true)
		{
    		curBar = TimeInc(curBar,cycle);
    		if(strcmp(exName.c_str(),"CFFEX")==0)
    		{
    			if(curBar >= 151500)
    			{
    			    break;
    			}
    			if((curBar >= 113000)&&(curBar <130000))
    			{
    				continue;
    			}
    		}
    		else 
    		{
    			if(curBar >= 150000)
    			{
    				break;
    			}
    			if((curBar >= 113000)&&(curBar <133000))
    			{
    				if((curBar == 130000)&(cycle == 60))
    				{
						barseq.push_back(133000);
    					continue;
    				}
    				else
    				{
        				continue;
    				}
    			}
    			if((curBar >= 101500)&&(curBar <103000))
    			{
    				continue;
    			}
    		}
    		barseq.push_back(curBar);
		}
		return barseq;
};
    
int CloseIndexUnit::TimeInc(int itime,int cycle)
{
	
		toolsClass tc;

		string sitime  = tc.IntToString(itime);
	    if(itime>=100000)
		{
			
		}else
		{
			sitime = "0"+sitime;
		}
		if(sitime.size()==6)
		{
		
			 string shour = sitime.substr(0,2);
			 string smin = sitime.substr(2,2);
			 string sss = sitime.substr(4,2);
			 int ishour = tc.stringToInt(shour);
			 int ismin = tc.stringToInt(smin);
			 int isss  = tc.stringToInt(sss);
			 int iTotalSecond = ishour*3600+ismin*60+isss;
			 int nowTotalSecond = iTotalSecond + cycle*60;
			 int inewHour = nowTotalSecond/3600;
			 int inewMin  = (nowTotalSecond-inewHour*3600)/60;
			 int inewSS  = (nowTotalSecond - inewHour*3600 - inewMin*60);
			 int newTime = inewHour*10000+inewMin*100+inewSS;
			 return newTime;

		}

		return -1;//fail
};
   
void CloseIndexUnit::computeIndex(TickData sec){

	if(strncmp(sec.getSec().c_str(),this->Sec.c_str(), Sec.length()) != 0){

		string tmpstr = sec.getSec();
		string tmpstr1 = tmpstr.substr(0,Sec.size());
		if(strcmp(tmpstr.c_str(),Sec.c_str())!=0)
		{
			return;
		}
        
    }
	toolsClass tc;
	// if(!sec.getDate().equals(date))
	if(strcmp(sec.getDate().c_str(),this->date.c_str())!=0){
        	 
        if(strcmp(this->date.c_str(),"default")!=0)//if(!date.equals("default"))
        {
            if(this->barSeq.size()-1-lastBarNum>0)
            {
				list<int>::iterator barSeqit = barSeq.begin();
				bool isLooped = false;
            	for(int l=lastBarNum+1;l<this->barSeq.size();l++)
            	{
					
            		int timec ;
					if(isLooped == false)
					{
						for(int i = 0;i!=l;i++)
						{
							barSeqit++;
						}
						isLooped = true;
					}
					timec = *barSeqit;
					IndexValue  invtmp(date,tc.IntToString(timec),0.0);
					indexListClose.push_back(invtmp);
					len++;
            	}
            	lastBarNum = this->barSeq.size() - 1;
            }
        }   
        lastMin = 0;
        lastBarNum = 0;
        date = sec.getDate();
    }
		
	int tmp = timeToHourmin(sec.getTime());
		 
	if (tmp < iInitHHMM) {
		return;
	}
	if(strcmp(this->exchangeName.c_str(),"CFFEX")==0)//if(this.exchangeName.equals("CFFEX"))
	{
		if(tmp >= 1515)
		{
			return;
		}
		if((tmp > 1130)&&(tmp <1300))
		{
			return;
		}
	}
	else 
	{
		if(tmp >= 1500)
		{
			return;
		}
		if((tmp > 1130)&&(tmp <1330))
		{
			return;
		}
		if((tmp >1015)&&(tmp <1030))
		{
			return;
		}
	}
	 
	int curBarTime = 0;
	int curBarNum = 0;
	list<int>::iterator itForBarSeq1;
	bool isLooped1 = false;
	for(curBarNum=0;curBarNum<this->barSeq.size();curBarNum++)
	{
		//int ktmp = getbarSeq(curBarNum);//int ktmp = this->barSeq.get(curBarNum);
		int ktmp;
		if(isLooped1 == false)
		{
			list<int>::iterator barSeqit = barSeq.begin();
			for(int i = 1;i<=curBarNum;i++)
			{
				barSeqit++;
			}
			itForBarSeq1 = barSeqit;
			isLooped1 =true;
		}
		ktmp = *itForBarSeq1;
		itForBarSeq1++;
		if(tmp*100 >= ktmp)
		{
			curBarTime = ktmp;
		}
		else
		{
			curBarNum = curBarNum -1;
			break;
		}
	}
	if(curBarNum >= this->barSeq.size())
	{
		curBarNum = this->barSeq.size()-1;
	}
	list<int>::iterator itForBarSeq2;
	bool isLooped2 = false;
	if (curBarTime != lastMin) 
	{
		if((lastMin == 0)&& curBarNum > 0) 
		{
			for(int k=0;k<curBarNum;k++)
			{
				//int timea = getbarSeq(k);
				int timea;
				if(isLooped2 == false)
				{
					list<int>::iterator barSeqit = barSeq.begin();
					for(int i = 1;i<=k;i++)
					{
						barSeqit++;
					}
					itForBarSeq2 = barSeqit;
					isLooped2 =true;
				}
				timea = *itForBarSeq2;
				itForBarSeq2++;
				IndexValue inv(sec.getDate(),""+timea,0.0);
				indexListClose.push_back(inv);
                len++;
				//IDR.IDR_println("HeadCover,"+this.name+","+this.paramlist.get(0)+","+sec.getDate()+","+""+timea);
			}
				
		}
		else if(curBarNum - lastBarNum > 1) 
		{
			list<int>::iterator itForBarSeq3;
			bool isLooped3 = false;
			for(int j=lastBarNum+1;j<curBarNum;j++)
			{
				//int timeb = getbarSeq(j);
				int timeb;
				if(isLooped3 == false)
				{
					list<int>::iterator barSeqit = barSeq.begin();
					for(int i = 1;i<=j;i++)
					{
						barSeqit++;
					}
					itForBarSeq3 = barSeqit;
					isLooped3 =true;
				}
				timeb = *itForBarSeq3;
				itForBarSeq3++;
				IndexValue inv(sec.getDate(),""+timeb,indexListClose.back().getValue());
				indexListClose.push_back(inv);
				len++; 
			}
		}
		close = sec.getLastPrice();
		IndexValue iv1;
		if(indexListClose.size()!=0)
		{
			iv1 = indexListClose.back();
			if(iv1.getTime().size()!=0)
			{
				cout<<"123"<<endl;
			}
		}
		
		IndexValue iv(sec.getDate(),sec.getTime(),(int)(close*10)/10.0);//IndexValue iv= new IndexValue(sec.getDate(),sec.getTime(),(int)(close*10)/10.0);
		indexListClose.push_back(iv);
		
		len++;
		lastMin = curBarTime;
		lastBarNum = curBarNum;
	}
	else
	{
		close = sec.getLastPrice();
		IndexValue iv(sec.getDate(),sec.getTime(),(int)(close*10)/10.0);
		
		indexListClose.pop_back();//indexListClose.remove(len - 1);
		indexListClose.push_back(iv);

	}
};

//only for client side
 
string CloseIndexUnit::getIUID() {
	return name;
	
};
   
//only for client side
	 
IndexValueBase* CloseIndexUnit::getIndex() {
	DoubleIndexValue* div =new DoubleIndexValue( indexListClose.back().getValue());
    return div;
};
    
//only for client side
IndexValueBase* CloseIndexUnit::getIndex(int index) {
	DoubleIndexValue* div;
	if (len + index < 0) {
		div = new DoubleIndexValue(0);
		return div;
	}
	int tmp = (len + index) % len;
	// return indexList.get(len - index - 1);
	div= new DoubleIndexValue(getindexListClosebyIndex(tmp).getValue());
	return div;
};
	
//only for client side
int CloseIndexUnit::getSize(){
     return indexListClose.size();     
};
    
//only for client side
 
void CloseIndexUnit::addIndexDetails(IndexDetailUnit idu) {
     IndexValue iv(idu.getDate(),idu.getTime(), idu.getValue());
	 indexListClose.push_back(iv);
     len++;
};
    
//only for client side
 
void CloseIndexUnit::clear() {
	lastMin = 0; 
};
int CloseIndexUnit::getbarSeq(int ibar)
{
	list<int>::iterator barSeqit = barSeq.begin();
	for(int i = 1;i<=ibar;i++)
	{
		barSeqit++;
	}
	return *barSeqit;
}
IndexValue  CloseIndexUnit::getindexListClosebyIndex(int index1)
{
	list<IndexValue>::iterator clit = indexListClose.begin();
	for(int i = 1;i<=index1;i++)
	{
		clit++;
	}
	return *clit;
}
