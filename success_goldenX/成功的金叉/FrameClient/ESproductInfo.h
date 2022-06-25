#pragma once
#include <string>
using namespace std;
#include "toolsClass.h"
class ESproductInfo
{
private:
	string strSep;
	int FieldsCount;
	toolsClass tc;
public:
	string Name; // eg. cu
	string Contract; //eg. cu888
	string Vol;
	string Strategy;
	string tn;
	string PosMult;
	string LossPrs;
	string tkPrs;
	string minMove;
	string pointValue;
	string commission;
	string ratioOfmargin;
	string proportion;
	string exchange;
	string tradingSessionString;
public:
	ESproductInfo();
	~ESproductInfo();
	bool string2object(string str);
};