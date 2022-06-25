#pragma once
#include <string>

using namespace std;

class ReqIndexUnit{
private :
	int Type;
	string IndexName;
	string ExIndexName;
	string Args;
	string EndOfDate;
	int Len;
public:
	ReqIndexUnit(int type, string indexname, string exindexname, string args, string endofdate, int len){
	    Type = type;
		IndexName = indexname;
		Args = args;
		EndOfDate = endofdate;
		Len = len;
		ExIndexName = exindexname;
	}
	~ReqIndexUnit(){
	}

	int getType(){
		return Type;
	}

	void setExIndexName(string value){
		ExIndexName = value;
	}

	string getExIndexName(){
		return ExIndexName;
	}

	string getIndexName(){
		return IndexName;
	}

	string getArgs(){
		return Args;
	}

	string getEndOfDate(){
		return EndOfDate;
	}

	int getLen(){
		return Len;
	}
};
