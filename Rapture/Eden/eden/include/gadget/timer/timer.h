/////////////////////////////////////////////////////////////////////////////
///@file	timer.h
///@brief	timer	define
////////////////////////////////////////////////////////////////////////////
#ifndef __TIMER_H__
#define __TIMER_H__

//#include <time.h>
#include <mmsystem.h>

EDENS_NAMESPACE_BEGIN

/**
*	@brief Caculate Time Class
*/
class CTimer
{
public:

	/**
	 * @brief タイマーの初期化
	 * 
	 */
	void SetUp();

	/**
	 * @brief タイマーのスタート
	 * 
	 */
	void RecordStartTime();

	/**
	 * @brief タイマーの現在時間を記録
	 * 
	 */
	void RecordCurrentTime();

	//u32 CalculateExecuteTime();

	/**
	 * @brief タイマーの記録時間を計算
	 * 
	 * \return 記録時間(second)
	 */
	double CalculateExecuteTime();

private:

	/** タイマーのスタート時間 */
	LARGE_INTEGER m_StartTime = {};

	/** タイマーの現在時間 */
	LARGE_INTEGER m_CurrentTime = {};
	
	/** CPUの周波数 */
	LARGE_INTEGER m_CPUFrequency = {};

};


EDENS_NAMESPACE_END

#endif