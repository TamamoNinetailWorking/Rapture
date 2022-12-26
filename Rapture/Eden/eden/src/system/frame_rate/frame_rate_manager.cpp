/////////////////////////////////////////////////////////////////////////////
///@file	frame_rate_manager.cpp
///@brief	CFrameRateManager	implementation
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////

#include <eden/include/system/frame_rate/frame_rate_manager.h>

#pragma comment(lib,"winmm.lib")

EDENS_NAMESPACE_BEGIN

void CFrameRateManager::Init(u32 frameRate,u32 resolution)
{
	m_TargetFrameRate = SCast<f32>(ONE_SECOND) / SCast<f32>(frameRate);
	m_nResolution = resolution;

	timeBeginPeriod(m_nResolution);

	m_FrameTimer.SetUp();

#ifdef EDENS_DEBUG
	m_MeasureTimer.SetUp();
#endif
	
	return;
}
void CFrameRateManager::Exit()
{
	timeEndPeriod(m_nResolution);
	return;
}

bool CFrameRateManager::CheckTime()
{
	m_FrameTimer.RecordCurrentTime();
	return m_FrameTimer.CalculateExecuteTime() >= m_TargetFrameRate;
}

void CFrameRateManager::UpdateTime()
{
#ifdef EDENS_DEBUG
	m_nCountFrame++;
#endif
	m_FrameTimer.RecordStartTime();
	return;
}

void CFrameRateManager::Measurement_FPS()
{

	m_MeasureTimer.RecordCurrentTime();

	if (m_MeasureTimer.CalculateExecuteTime() >= m_nMeasurePoint)
	{
		f32 DurationTime = SCast<f32>(m_nCountFrame * ONE_SECOND);
		f32 Delta = SCast<f32>(m_MeasureTimer.CalculateExecuteTime());
		m_FPS = DurationTime / Delta;
		m_MeasureTimer.RecordStartTime();
		m_nCountFrame = 0;
	}

	return;
}


EDENS_NAMESPACE_END