/////////////////////////////////////////////////////////////////////////////
///@file	timer.h
///@brief	timer	define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>

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

	u32 CalculateExecuteTime();

private:

	u32 m_StartTime = 0;
	u32 m_CurrentTime = 0;

};


EDENS_NAMESPACE_END

#endif