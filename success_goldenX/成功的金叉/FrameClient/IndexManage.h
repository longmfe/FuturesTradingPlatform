#pragma once
#include <string>
#include <map>
#include <list>
#include "IndexUnit.h"
using namespace std;
class IndexManage
{
public:
	IndexManage(void);
	~IndexManage(void);
	map<string, IndexUnit* > indexMap;
	list<IndexUnit*> iuList;

	
	
	//注册需要的指标
	void registerUnit(IndexUnit* iu);
	//指标的分笔计算
	void compute(TickData sec);
	IndexUnit* getIndexUnit(string id);
        
    void cleanForNewDay();
};

