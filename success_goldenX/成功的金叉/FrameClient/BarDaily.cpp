#include "StdAfx.h"
#include "BarDaily.h"
#include "BarValue.h"
#include "toolsClass.h"
#include "DoubleIndexValue.h"
#include "AlgoManager.h"
#include <cctype>
extern AlgoManager* algomanager;

BarDaily::BarDaily(string exname, string sec)
{
	name = "BarDaily";
	exchangeName = exname;
	secName = sec;
	date = "default";
	filename = "./Data/"+ getPz(sec) + "BarDaily.csv";
	this->strSep = ",";
	
	lastMin = 0; //time of current bar 
	lastBarNum = 0; //Num of current bar
	len = 0; // length of indexListClose
	open = 0.0;//current index value
	close = 0.0;//current index value
	high = 0.0;//current index value
	low = 0.0;//current index value
	tradevolume = 0.0;//current index value
	openinterest = 0.0;//current index value

	name += "" + sec;
	Sec = sec;
	date = "default";
	bwriteBD = false;

	readBDfromFile();
}

BarDaily::~BarDaily(void){}

string BarDaily::getIUID() {
	return name;
};

IndexValueBase* BarDaily::getIndex() {
	BarValue* bv =new BarValue( indexListBar.back());
    return bv;
};
IndexValueBase* BarDaily::getIndex(int index) {
	BarValue* bv;
	DoubleIndexValue* div;
	if (len + index < 0) {
		div = new DoubleIndexValue(0);
		return div;
	}
	int tmp = (len + index) % len;
	bv= new BarValue(getindexListBarbyIndex(tmp));
	return bv;
};

void BarDaily::computeIndex(TickData ss){
	int tmp = timeToHourmin(ss.getTime());
	
	//if( tmp >= 1500 && bwriteBD == false && algomanager->IsMDReal()){
		//bwriteBD = true;
		//writeBDtoFile(ss.getSec());
		//cout<< "writeBDtoFile at the end of day!!" <<endl;
	//}

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

	if(strcmp(ss.getDate().c_str(),this->date.c_str()) != 0 ){
		if(!algomanager->IsMDReal() && strcmp(this->date.c_str(),"default") != 0){
			writeBDtoFile(this->date);
		}
		bwriteBD = false;

		this->open = ss.getLastPrice();
		this->close = ss.getLastPrice();
		this->high = ss.getLastPrice();
		this->low = ss.getLastPrice();
		this->tradevolume = ss.getLastVol();
		this->openinterest = ss.getOpenInt();
		BarValue bv (ss.getDate(), "", this->open, this->close, this->high, this->low,
			this->tradevolume, this->openinterest, ss.getSec(), this->Sec);
		indexListBar.push_back(bv);
		len++;
		this->date = ss.getDate();
	}else{
		this->close = ss.getLastPrice();
		if(this->high < ss.getLastPrice()) this->high = ss.getLastPrice();
		if(this->low > ss.getLastPrice()) this->low = ss.getLastPrice();
		this->tradevolume += ss.getLastVol();
		this->openinterest = ss.getOpenInt();
		BarValue bv (ss.getDate(), "", this->open, this->close, this->high, this->low,
			this->tradevolume, this->openinterest, ss.getSec(), this->Sec);
		indexListBar.pop_back();
		indexListBar.push_back(bv);
	}
}

BarValue  BarDaily::getindexListBarbyIndex(int index1)
{
	list<BarValue>::iterator clit = indexListBar.begin();
	for(int i = 1;i<=index1;i++)
	{
		clit++;
	}
	return *clit;
}

int BarDaily::timeToHourmin(string time) {
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

void BarDaily::addIndexDetails(IndexDetailUnit idu)
{

};
void BarDaily::clear() {
	//lastMin = 0; 
};

int BarDaily::getlen(){
	return len;
}

void BarDaily::addTodayBD(string date, double value){
	
	//map<string ,double >::iterator it = ATRmap.find(date);
	//if(it == ATRmap.end())
	//{
	//	ATRmap.insert(pair<string,double>(date, value));
		//writeATRtoFile(date, value);
	//}
}

void BarDaily::readBDfromFile(){
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
			BarValue bv (info[0], "", atof(info[1].c_str()), atof(info[2].c_str()), atof(info[3].c_str()), atof(info[4].c_str()),
				atof(info[5].c_str()),atof(info[6].c_str()), "", this->Sec);
			indexListBar.push_back(bv);
			len++;
		}
	}
	infile.close();
}

void BarDaily::writeBDtoFile(string thesec){
	if(strcmp(this->date.c_str(),"defalut") == 0){
		return;
	}

	ifstream ifile(filename);
	if(ifile.is_open()){
		string str;
		while(ifile >> str){ 	
			if(str.find(this->date)!=std::string::npos){
				ifile.close();
				return;
			}
		} 		
	} 	
	ifile.close();

	string bdstr;
	bdstr = this->date + this->strSep +  
	   tc.doubleToString(this->open) + this->strSep +  
	   tc.doubleToString(this->close) + this->strSep + 
	   tc.doubleToString(this->high) + this->strSep +  
	   tc.doubleToString(this->low) + this->strSep +   
	   tc.doubleToString(this->tradevolume) + this->strSep +   
	   tc.doubleToString(this->openinterest);

	BarValue bv (this->date, "", this->open, this->close, this->high, this->low,
			this->tradevolume, this->openinterest, thesec, this->Sec);
	indexListBar.push_back(bv);
	len++;

	ofstream outfile(filename,ios::app);  //outfile.open;
	if(outfile.is_open()){
		outfile<<bdstr<<endl;
	}
	outfile.close();
	cout<< "writeBDtoFile!!" + this->date <<endl;
}

string BarDaily::getPz(string con)
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