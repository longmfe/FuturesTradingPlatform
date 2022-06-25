#pragma once
#include "computeindexunit.h"
#include "IndexManage.h"	
#include "GlobalDataManage.h"
#include "CloseIndexUnit.h"
class GlobalDataManage;

class ComputeIndex :
	public ComputeIndexUnit
{
public:
	ComputeIndex(GlobalDataManage*, string conID);
	~ComputeIndex(void);
	IndexManage* im;
	virtual void init();
	virtual void run();
private:
	GlobalDataManage* m_ma;
	string contractID;
};

