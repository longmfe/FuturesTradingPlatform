#pragma once
#include "Order.h"
class SendOrder{
public:
	SendOrder(){
	}
	~SendOrder(){
	}
	virtual bool sendOrder(Order* or) = 0;
};