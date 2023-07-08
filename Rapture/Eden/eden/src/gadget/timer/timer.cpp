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

void CTimer::SetUp()
{
	//m_StartTime = m_CurrentTime = timeGetTime();
	QueryPerformanceFrequency(&m_CPUFrequency);
}

void CTimer::RecordStartTime()
{
	//m_StartTime = timeGetTime();
	QueryPerformanceCounter(&m_StartTime);
}

void CTimer::RecordCurrentTime()
{
	//m_CurrentTime = timeGetTime();
	QueryPerformanceCounter(&m_CurrentTime);
}

double CTimer::CalculateExecuteTime()
{
	return (SCast<double>(m_CurrentTime.QuadPart) - SCast<double>(m_StartTime.QuadPart)) / m_CPUFrequency.QuadPart;
}


EDENS_NAMESPACE_END