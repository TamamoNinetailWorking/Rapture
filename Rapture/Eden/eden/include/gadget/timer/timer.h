/////////////////////////////////////////////////////////////////////////////
///@file	timer.h
///@brief	timer	define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H__
#define __TIMER_H__

//#include <time.h>
#include <mmsystem.h>

EDENS_NAMESPACE_BEGIN

/*
*	Caculate Time Class
*/
class CTimer
{
public:

	void SetUp();

	void RecordStartTime();

	void RecordCurrentTime();

	//u32 CalculateExecuteTime();
	double CalculateExecuteTime();

private:

	LARGE_INTEGER m_StartTime = {};
	//u32 m_StartTime = 0;
	LARGE_INTEGER m_CurrentTime = {};
	//u32 m_CurrentTime = 0;
	LARGE_INTEGER m_CPUFrequency = {};

};


EDENS_NAMESPACE_END

#endif