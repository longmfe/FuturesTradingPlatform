#include "StdAfx.h"
#include "Bar.h"
#include "BarValue.h"
#include"toolsClass.h"
#include"DoubleIndexValue.h"
Bar::Bar(int min, int initHHMM, string sec)
{
	name = "Bar";
	filename = "./Data/"+ getPz(sec) + "1minBar.csv";
	lastMin = 0; //time of current bar 
	lastBarNum = 0; //Num of current bar
	len = 0; // length of indexListClose
	open = 0.0;//current index value
	close = 0.0;//current index value
	high = 0.0;//current index value
	low = 0.0;//current index value
	tradevolume = 0.0;//current index value
	openinterest = 0.0;//current index value
	date = "default";
	inValidV = 0;	
	this->strSep = ",";
    name += "" + tc.IntToString(min) + sec;
	iInitHHMM = initHHMM;
	this->min = min;
	Sec = sec;
	if(915 == initHHMM)
	{
		exchangeName = "CFFEX";
	}
	else
	{
		exchangeName = "Not_CFFEX";
	}
	this->barSeq = this->GetBarSeq(this->iInitHHMM*100, this->min, this->exchangeName);
	this->IsNumInc = false;
	//this->date = "default";

	this->Iszerofill = false;

	this->isFirst=false;

	readBarfromFile();	
}
Bar::Bar(list<string> paramlist){}
Bar::~Bar(void){}
int Bar::timeToHourmin(string time) {
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
list<int> Bar::GetBarSeq(int InitTime,int cycle,string exName) 
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
int Bar::TimeInc(int itime,int cycle)
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

void Bar::computeIndex(TickData sec){
	if(strncmp(sec.getSec().c_str(),this->Sec.c_str(), Sec.length()) != 0){
		string tmpstr = sec.getSec();
		string tmpstr1 = tmpstr.substr(0,Sec.size());
		if(strcmp(tmpstr.c_str(),Sec.c_str())!=0){
			return;
		}        
    }
	toolsClass tc;	
	if(strcmp(sec.getDate().c_str(),this->date.c_str())!=0){        	 
        if(strcmp(this->date.c_str(),"default")!=0){	
			indexListBar.clear();
			len = 0;
			readBarfromFile();
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
					if(this->Iszerofill)
					{
						BarValue* invtmp=new BarValue(date, "" + tc.IntToString(timec), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, sec.getSec(), Sec);
					    indexListBar.push_back(invtmp);
						len++;
						hasOpenList.push_back(false);
						hasOpenShortList.push_back(false);
					}
					
            	}
            	lastBarNum = this->barSeq.size() - 1;
            }
        }   
        lastMin = 0;
        lastBarNum = 0;
        date = sec.getDate();
    }
		
	int tmp = timeToHourmin(sec.getTime());   //只取小时和分钟，最多4位数
		 
	if (tmp < iInitHHMM) {
		return;
	}

	if(strcmp(this->exchangeName.c_str(),"CFFEX")==0)
	{
		if(tmp > 1515)
		{
			return;
		}
		if((tmp >= 1130)&&(tmp <1300))
		{
			return;
		}
		if(tmp < 915)
		{
	       return;
		}
	}
	else 
	{
		if(tmp > 1500)
		{
			return;
		}
		if((tmp >= 1130)&&(tmp <1330))
		{
			return;
		}
		if((tmp >=1015)&&(tmp <1030))
		{
			return;
		}
		if(tmp < 859)
		{
			return;
		}
	}
	 
	int curBarTime = 0;
	int curBarNum = 0;
	list<int>::iterator itForBarSeq1;
	bool isLooped1 = false;
	int barSeq_size=this->barSeq.size();
	for(curBarNum=0;curBarNum<barSeq_size;curBarNum++)
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
	if (curBarTime != lastMin)    //分钟不同，新的bar
	{
		if(curBarTime >= 143000)
			writeBartoFile(sec.getSec());

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
				if(this->Iszerofill)
				{
				    BarValue* inv=new BarValue(sec.getDate(),""+timea, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, sec.getSec(), this->Sec);
				    indexListBar.push_back(inv);
					len++;
					hasOpenList.push_back(false);
					hasOpenShortList.push_back(false);
				}
             
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
				if(this->Iszerofill){
					//this fulfill rule is from QRC, in order to adapt to QRC project.2013-04-23 Aaron.
				    BarValue* inv=new BarValue(sec.getDate(),""+timeb,indexListBar.back()->close,indexListBar.back()->close,indexListBar.back()->close,
					indexListBar.back()->close,0,0,sec.getSec(),this->Sec);

				    indexListBar.push_back(inv);
				    len++;
					hasOpenList.push_back(false);
					hasOpenShortList.push_back(false);
				}
			}
		}
		 this->open = sec.getLastPrice();
            this->close = sec.getLastPrice();
            this->high = sec.getLastPrice();
            this->low = sec.getLastPrice();
            this->tradevolume = sec.getLastVol();
            this->openinterest = sec.getOpenInt();
        /*
		IndexValue iv1;
		if(indexListClose.size()!=0)
		{
			iv1 = indexListClose.back();
			if(iv1.getTime().size()!=0)
			{
				cout<<"123"<<endl;
			}
		}
		*/
		BarValue* bv=new BarValue(sec.getDate(), sec.getTime(),
                    (int) (this->open * 10) / 10.0,
                    (int) (this->close * 10) / 10.0,
                    (int) (this->high * 10) / 10.0,
                    (int) (this->low * 10) / 10.0,
                    (int) (this->tradevolume * 10) / 10.0,
                    (int) (this->openinterest * 10) / 10.0,
                    sec.getSec(),
                    this->Sec);
		indexListBar.push_back(bv);
		len++;
		hasOpenList.push_back(false);
		hasOpenShortList.push_back(false);
		lastMin = curBarTime;   //更新最新的分钟为现在的分钟数, 乘了100，但精确到分钟  zengty注
		lastBarNum = curBarNum;  //更新最新的bar数目（不一定是一分钟一个）  zengty注
		isFirst=true;

		if(indexListBar.size()>1)
			this->IsNumInc = true;

	}else{   //属于同一分钟，更新当前的bar
		isFirst=false;
		this->close = sec.getLastPrice();
        if (this->high < sec.getLastPrice()) {
            this->high = sec.getLastPrice();
        }
        if (this->low > sec.getLastPrice()) {
            this->low = sec.getLastPrice();
        }
        this->tradevolume += sec.getLastVol();
        this->openinterest = sec.getOpenInt();

        BarValue* bv =new BarValue(sec.getDate(), sec.getTime(),
                (int) (this->open * 10) / 10.0,
                (int) (this->close * 10) / 10.0,
                (int) (this->high * 10) / 10.0,
                (int) (this->low * 10) / 10.0,
                (int) (this->tradevolume * 10) / 10.0,
                (int) (this->openinterest * 10) / 10.0,
                sec.getSec(),
                this->Sec);
		BarValue* bv_pre=indexListBar.back();
		indexListBar.pop_back();//indexListClose.remove(len - 1);
		indexListBar.push_back(bv);
		delete bv_pre;

	}
};

bool Bar::getIsFirst(){
	return isFirst;
}

string Bar::getIUID() {
	return name;
};
IndexValueBase* Bar::getIndex() {
	BarValue* bv = indexListBar.back();
    return bv;
};
IndexValueBase* Bar::getIndex(int index) {
	BarValue* bv;
	DoubleIndexValue* div;
	if (len + index < 0) {
		div = new DoubleIndexValue(0);
		return div;
	}
	int tmp = (len + index) % len;
	// return indexList.get(len - index - 1);
	bv= getindexListBarbyIndex(tmp);
	return bv;
};

bool Bar::getHasOpen(int index){
	list<bool>::iterator clit = hasOpenList.begin();
	for(int i = 1;i<=index;i++)
	{
		clit++;
	}
	return *clit;
}

bool Bar::getLastFlag(){
	list<bool>::iterator it=hasOpenList.begin();
	for(int i=1;i<hasOpenList.size()-1;i++){
		it++;
	}
	return *it;
}

bool Bar::getHasOpenShort(int index){
	list<bool>::iterator clit = hasOpenShortList.begin();
	for(int i = 1;i<=index;i++)
	{
		clit++;
	}
	return *clit;
}

bool Bar::getLastShortFlag(){
	list<bool>::iterator it=hasOpenShortList.begin();
	for(int i=1;i<hasOpenShortList.size()-1;i++){
		it++;
	}
	return *it;
}

/*
IndexValueBase* Bar::getBVbyHHMM(string myHHMM)
{
	
	list <BarValue>::reverse_iterator rIt; 
	for (rIt=indexListBar.rbegin(); rIt!=indexListBar.rend(); rIt++)
	{ 
		if(*rIt->time.compare(0,4,HHMM,4)==0)
		{
	        return rIt;
		}
		else if(*rIt->time.compare(0,4,HHMM,4)> 0)
		{
	        continue;
		}
		else if((*rIt->time.compare(0,4,HHMM,4)< 0)
		{
	        DoubleIndexValue* div;
			div = new DoubleIndexValue(0);
			return div;
		}
	}
	
};*/
BarValue*  Bar::getindexListBarbyIndex(int index1)
{
	list<BarValue*>::iterator clit = indexListBar.begin();
	for(int i = 1;i<=index1;i++)
	{
		clit++;
	}
	return *clit;
}
int Bar::getSize(){
	if(&indexListBar == NULL)
	{
	return 0;
	}
	if(indexListBar.empty())
	{
	   return 0;
	}
	else
     return indexListBar.size();     
};
void Bar::addIndexDetails(IndexDetailUnit idu)
{

};
void Bar::clear() {
	lastMin = 0; 
};
bool Bar::getIncStatus()
{
	return this->IsNumInc;

}
void Bar::setIncKnown()
{

	this->IsNumInc = false;
}
string Bar::getSec()
{

	return this->Sec;
};


void Bar::readBarfromFile(){
	char buf[512];                
	ifstream infile;
	infile.open(filename);
	
	if(infile.is_open())          //open successfully?
	{
		//略过第一行
		if(infile.good() && !infile.eof())
		{
			infile.getline(buf,sizeof(buf)); 
		}

		while(infile.good() && !infile.eof())
		{
			memset(buf,0,sizeof(buf));
			infile.getline(buf,sizeof(buf)); 
			string  str = buf ;
			if(str.length() < 1)
			{
				continue;
			}

			toolsClass tc;
			vector<string>  info = this->tc.split(str,",", "<null>");

			//Date, Open, Close, High, Low,tradevolume,openinterest
			BarValue* bv =new BarValue(info[0], info[1], atof(info[2].c_str()), atof(info[3].c_str()), atof(info[4].c_str()), atof(info[5].c_str()),
				atof(info[6].c_str()),atof(info[7].c_str()), info[8], this->Sec);
			indexListBar.push_back(bv);
			len++;
			hasOpenList.push_back(false);
			hasOpenShortList.push_back(false);
		}
	}
	infile.close();

	 ofstream out; 
	 out.open(filename); 
	 out.clear(); 
	 out.close();
}

void Bar::writeBartoFile(string secname){
	if(strcmp(this->date.c_str(),"defalut") == 0){
		return;
	}

	string bdstr;
	bdstr = this->date + this->strSep +
		tc.IntToString(this->lastMin) +  this->strSep +
	   tc.doubleToString(this->open) + this->strSep +  
	   tc.doubleToString(this->close) + this->strSep + 
	   tc.doubleToString(this->high) + this->strSep +  
	   tc.doubleToString(this->low) + this->strSep +   
	   tc.doubleToString(this->tradevolume) + this->strSep +   
	   tc.doubleToString(this->openinterest) + this->strSep + secname + this->strSep + this->Sec ;

	//BarValue bv (this->date, "", this->open, this->close, this->high, this->low,
	//		this->tradevolume, this->openinterest, thesec, this->Sec);
	//indexListBar.push_back(bv);
	//len++;

	ofstream outfile(filename,ios::app);  //outfile.open;
	if(outfile.is_open()){
		outfile<<bdstr<<endl;
	}
	outfile.close();
}

string Bar::getPz(string con)
{
	string res;
	//判断第二个字符是否为数字，如果是数字则只有1个字符是品种名称；如果不是数字，则前两个字符为品种名称
	if(con.at(1) >= 'A')
	{
		res += con.at(0);
		res += con.at(1);
	}
	else
	{
		res = con.at(0);
	}

	//如果不符合规则，返回空
	if(con.length() > 3 && con.at(2) >= 'A')
	{
		res = "";
	}
	for (int i = 0; i < res.length(); ++i)        
		res[i] = tolower(res[i]);
	return res;
}

int Bar::getLastBarNum(){
	return lastBarNum;
}