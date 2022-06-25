#ifndef STRINGUTIL_H
#define STRINGUTIL_H
//#include "stdafx.h"
#include <string>
#include <list>
using namespace std;

class StringUtil{
public:
	StringUtil(){
	}
	~StringUtil(){
	}

	list<string> stringAnalysis(string linestring, string split){
		list<string> tmpl;
		string tmp = "";
		
		for(int i = 0 ; i <= linestring.length(); i ++){
			if(i == linestring.length() || strncmp(&linestring[i],split.c_str(),1) == 0 ){
				tmpl.push_back(tmp);
				tmp = "";
			}
			else{
				tmp += linestring[i];
			}
		}
		
		return tmpl;
	}
	list<string> stringAnalysisByComma(string linestring);
};

#endif