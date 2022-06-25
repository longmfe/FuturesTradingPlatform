/*****************************************************************************
Writren by : Liu qin tao 
******************************************************************************/
#pragma once
#include<string>
#include <sstream>
#include <vector>
using namespace std;
class toolsClass
{
public:
	toolsClass(void);
	~toolsClass(void);
    double stringToDouble(std::string const& s);
	string doubleToString(double value);
	string deleteSubStr(string srcstr,char car1);
	int stringToInt(string str);
	string IntToString(int itmp);
	string&   replace_all_distinct(string&   str,const   string&   old_value,const   string&   new_value);
	string getPreStrBySplit(string src,string split);
	vector<string> split(const string& src, string delimit,string null_subst="");
	int timeToHourmin(string time);
};

