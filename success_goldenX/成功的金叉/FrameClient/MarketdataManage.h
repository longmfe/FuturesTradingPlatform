#pragma once
#include <map>
#include <list>
#include <string>
#include "TickData.h"
#include "Position.h"
#include "AlgoTSEng.h"
#include "GlobalDataManage.h"
using namespace std;
class Position;
class GlobalDataManage;
class MarketdataManage
{
public:
	MarketdataManage(GlobalDataManage*);
	~MarketdataManage(void);
	string firstCon;
    list<TickData*> secList;
    map<string, list<TickData*>> secMap;


    void setDefaultSec(string con);

    TickData* getTickDataByIndex(int index);

    TickData* getTickDataByIndex(string con, int index);

    void addTickData(TickData* sec);

    void addTickDatas(TickData* sec);

    int getLen();
    int getLen(string con);

    TickData* getTickData();

    TickData* getTickData(string con);
	//TickData* getSec();
private :
	GlobalDataManage* m_ma;
};

