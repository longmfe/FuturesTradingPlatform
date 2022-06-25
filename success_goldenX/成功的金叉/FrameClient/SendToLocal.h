#pragma once
#include "StdAfx.h"
#include "SendOrder.h"
#include "Order.h"
using namespace std;
class GlobalDataManage;

class SendToLocal:SendOrder
{
public:
	SendToLocal(GlobalDataManage* pGlobalDm);
	~SendToLocal();
	virtual bool sendOrder(Order* or);
private:
	GlobalDataManage* m_pGlobalDm;
};