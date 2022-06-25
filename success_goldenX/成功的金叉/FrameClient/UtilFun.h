#pragma once
#include <string>
#include <Windows.h>
using namespace std;


class UtilFun{
public:
	UtilFun(){
	}
	~UtilFun(){
	}

	string getCurDate(){		
		return "";
	}

	string getCHCurDate(){
		SYSTEMTIME sys; 
		GetLocalTime( &sys );
		int tmp = sys.wYear * 10000 + sys.wMonth * 100 +  sys.wDay;
		int tmp1 = sys.wHour * 10000 + sys.wMinute * 100 + sys.wSecond;

		char strs[12] = {0};
		_itoa(tmp, strs, 10);
		string str = string(strs);

		memset(strs, 0 , sizeof(strs));
		_itoa(tmp1, strs, 10);

		string strtmp = "";

		if(sys.wHour < 10){
			strtmp = "0";
		}
		strtmp += string(strs);

		str += '-';

		str += strtmp;

		return str;
	}

	int getHHMMSS(){
		SYSTEMTIME sys; 
		GetLocalTime( &sys );
		int tmp = sys.wHour * 10000 + sys.wMinute * 100 + sys.wSecond;

		return tmp;
	}

	string getCHCurYYYYMM(){
		return "";
	}

	string getCHCurMMDD(){
		return "";
	}

	string getCHCurYYYMMDD(){
		return "";
	}

	bool IsWeekend(){
		bool res = true;
		SYSTEMTIME sys; 
		GetLocalTime( &sys );
		int days = CaculateWeekDay(sys.wYear, sys.wMonth, sys.wDay);
		if(days <= 4){
			res = false;
		}
		return res;
	}

private:
	//·µ»ØÖÜ¼¸
	int CaculateWeekDay(int y,int m, int d)
	{
		if(m==1||m==2) {
			m+=12;
			y--;
		}
		return (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;	
	} 
};