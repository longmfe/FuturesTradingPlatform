#pragma once
#include "DealSendedOrderUnit.h"
#include "GlobalDataManage.h"
class GlobalDataManage;
class DealOrdersLocal:DealSendedOrderUnit{
public:
	DealOrdersLocal(GlobalDataManage* );
	~DealOrdersLocal();
	virtual void init();
	virtual void run();
private:
	GlobalDataManage*  m_ma;
};