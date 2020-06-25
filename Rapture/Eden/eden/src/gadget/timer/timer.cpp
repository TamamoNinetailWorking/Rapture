/////////////////////////////////////////////////////////////////////////////
///@file	timer.cpp
///@brief	timer	implementation
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////

//Include Header
#include <eden/include/gadget/timer/timer.h>

//Using Library
#pragma comment(lib,"winmm.lib")

EDENS_NAMESPACE_BEGIN

void CTimer::Initialize()
{
	m_StartTime = m_CurrentTime = timeGetTime();
}

void CTimer::RecordStartTime()
{
	m_StartTime = timeGetTime();
}

void CTimer::RecordCurrentTime()
{
	m_CurrentTime = timeGetTime();
}


EDENS_NAMESPACE_END