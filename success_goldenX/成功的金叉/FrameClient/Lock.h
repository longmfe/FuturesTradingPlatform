#pragma once
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

