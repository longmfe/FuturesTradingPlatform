#pragma once
#include <string>
#include "TickData.h"
#include "IndexDetailUnit.h"
#include "IndexValueBase.h"
using namespace std;
 class IndexUnit
{
private:
	 
public:
	IndexUnit(string sid);
	IndexUnit();
	~IndexUnit(void);
	virtual string getIUID()=0;
	
	virtual IndexValueBase* getIndex()=0;

	virtual IndexValueBase* getIndex(int index)=0;
	
	virtual void computeIndex(TickData sec)=0;
        
    virtual void addIndexDetails(IndexDetailUnit idu)=0;
        
    virtual void clear()=0;
};

