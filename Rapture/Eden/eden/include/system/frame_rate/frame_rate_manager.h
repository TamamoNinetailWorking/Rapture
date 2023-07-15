/////////////////////////////////////////////////////////////////////////////
///@file	frame_rate_manager.h
///@brief	CFrameRateManager	define
///@data	2020/06/26
////////////////////////////////////////////////////////////////////////////
#ifndef __FRAME_RATE_MANAGER_H__
#define __FRAME_RATE_MANAGER_H__

//Include Header
#include <eden/include/template/singleton.h>
#include <eden/include/gadget/timer/timer.h>

EDENS_NAMESPACE_BEGIN


/*
*	@brief フレームレート管理クラス
*	@note 旧システムの一部のため、現在は使用していない
*/
class CFrameRateManager : public CSingleton<CFrameRateManager>
{
protected:
	
	friend class CSingleton<CFrameRateManager>;

public:

	void Init(u32 frameRate,u32 resolution = 1);

	void Exit();


	b8 CheckTime();
	

	void UpdateTime();

#ifdef EDENS_DEBUG

	void Measurement_FPS();

	f32 GetFPS() const { return m_FPS; };

#endif

protected:

	const u32 ONE_SECOND = 1000;//1s
	f32 m_TargetFrameRate = 0.0f;
	u32 m_nMeasurePoint = ONE_SECOND;//time_measure_point

	u32 m_nResolution = 1;

	s32 m_nCountFrame = 0;
	f32 m_FPS = 0.0f;

	CTimer m_FrameTimer;

#ifdef EDENS_DEBUG
	
	CTimer m_MeasureTimer;

#endif
};

EDENS_NAMESPACE_END

#endif