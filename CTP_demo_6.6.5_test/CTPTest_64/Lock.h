#pragma once
//#include <WinBase.h>
#include <Windows.h>
class CLock
{
public:
	CLock(void);
	~CLock(void);
private:
	CRITICAL_SECTION cs;
public:
	void lock(void);
	void unlock(void);
};

