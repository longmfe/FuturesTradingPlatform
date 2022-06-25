#include "StdAfx.h"
#include "IndexManage.h"


IndexManage::IndexManage(void)
{

}


IndexManage::~IndexManage(void)
{
	//map<string, IndexUnit* > indexMap;
	map<string, IndexUnit* >::iterator it =  indexMap.begin();
	if(it!= indexMap.end())
	{
		for(;it!=indexMap.end();it++)
		{
			if(it->second!=NULL)
			{
				IndexUnit* tmp = it->second;
				delete tmp;
				tmp = NULL;
			}
		}
		
	}
	
}
void IndexManage::registerUnit(IndexUnit* iu)
{
	map<string ,IndexUnit*>::iterator it = indexMap.find(iu->getIUID());
	if(it == indexMap.end())
	{
		indexMap.insert(pair<string,IndexUnit*>(iu->getIUID(),iu));
		iuList.push_back(iu);
	}else
	{
		delete iu;
	}
}

void IndexManage::compute(TickData sec)
{
	list<IndexUnit*>::iterator it = iuList.begin();
	for(;it!=iuList.end();it++)
	{
		(*it)->computeIndex(sec);
	}
}
IndexUnit* IndexManage::getIndexUnit(string id)
{
    map<string ,IndexUnit*>::iterator it = indexMap.find(id);
	if(it == indexMap.end())
	{
		return NULL;
	}else
	{
		return it->second;
	}
}
void IndexManage::cleanForNewDay()
{
	map<string, IndexUnit* >::iterator it =  indexMap.begin();
	for(;it!=indexMap.end();it++)
	{
		if(it->second!=NULL)
		{
			delete it->second;
		}
	}
	indexMap.clear();
	iuList.clear();
	// list 是否也需要 clear();

}

