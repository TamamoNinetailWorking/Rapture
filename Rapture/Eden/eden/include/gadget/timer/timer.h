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

	void Initialize();

	void RecordStartTime();

	void RecordCurrentTime();

private:

	uquad m_StartTime = 0;
	uquad m_CurrentTime = 0;

};


EDENS_NAMESPACE_END

#endif