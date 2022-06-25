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

	
	
	//ע����Ҫ��ָ��
	void registerUnit(IndexUnit* iu);
	//ָ��ķֱʼ���
	void compute(TickData sec);
	IndexUnit* getIndexUnit(string id);
        
    void cleanForNewDay();
};

