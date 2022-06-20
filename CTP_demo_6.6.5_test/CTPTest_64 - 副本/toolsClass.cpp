#include "StdAfx.h"
#include "toolsClass.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
using namespace std;
typedef basic_string<char>::size_type S_T;  
static const S_T npos = -1; 

toolsClass::toolsClass(void)
{
}


toolsClass::~toolsClass(void)
{
}
class BadConversion : public std::runtime_error {
public:
BadConversion(std::string const& s)
: std::runtime_error(s)
{ }
};
double toolsClass::stringToDouble(std::string const& s)
{
std::istringstream i(s);
double x;
if (!(i >> x))
throw BadConversion("convertToDouble(\"" + s + "\")");
return x;
}
string toolsClass::doubleToString(double value)
{
	stringstream ss; 
	string result; 
	ss << value; 
	ss >>result;  
	return result;
}
string  toolsClass::deleteSubStr(string srcstr,char car1)
{
	string abc = srcstr;
	while(1)
	{
		int nPos = abc.find_first_of(car1);
		if(nPos !=string::npos)
		{
			abc = abc.substr(0,nPos) + abc.substr(nPos+1,-1);
		}else
			break;
	}
	return abc;
	
}
int  toolsClass::stringToInt(string str)
{
	string text = str;
    int number = std::atoi( text.c_str() );
	return number;
}
string toolsClass::IntToString(int itmp)
{
	string c;
	ostringstream oss;
	oss<<itmp;
	c=oss.str();
	return c;
}
string&   toolsClass::replace_all_distinct(string&  str,const   string&   old_value,const   string&   new_value)   
{   
    for(string::size_type   pos(0);   pos!=string::npos;   pos+=new_value.length())   {   
        if(   (pos=str.find(old_value,pos))!=string::npos   )   
            str.replace(pos,old_value.length(),new_value);   
        else   break;   
    }   
    return   str;   
}  
string  toolsClass::getPreStrBySplit(string src,string split)
{
	 string sub = src;
	 //sub.find(
	 int i = sub.find(split);
	 string sub1  = sub.substr(0,i);
	 return sub1;
}
vector<string> toolsClass::split(const string& src, string delimit,string null_subst)    
{    
     if( src.empty() || delimit.empty() ) throw "split:   empty string 0";    
   
     vector<string> v;    
     S_T deli_len = delimit.size();    
     long index = npos, last_search_position = 0;    
     while( (index=src.find(delimit,    

                  last_search_position))!=npos )    
     {    
         if(index==last_search_position)    
             v.push_back(null_subst);    
         else   
            v.push_back( src.substr(last_search_position, index-    
   
                                     last_search_position) );    
            last_search_position = index + deli_len;    
      }    
     string last_one = src.substr(last_search_position);    
     v.push_back( last_one.empty()? null_subst:last_one );    
     return v;    
}    
//return like 101100
int toolsClass::timeToHourmin(string time) {
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
	return result*100;
};