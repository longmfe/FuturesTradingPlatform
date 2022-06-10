#include "StdAfx.h"
#include "Lock.h"


CLock::CLock(void)
{
	InitializeCriticalSection(&cs);
}


CLock::~CLock(void)
{
	DeleteCriticalSection(&cs);
}


void CLock::lock(void)
{
	EnterCriticalSection( & cs);
}


void CLock::unlock(void)
{
	LeaveCriticalSection( & cs);
}
