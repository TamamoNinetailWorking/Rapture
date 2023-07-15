#include "UpdateIntervalSubsystem.h"
#include "UpdateIntervalSubsystemInitializer.h"

#include <eden/include/gadget/timer/timer.h>

#include <eden/include/utility/ender_utility.h>

USING_BIFROST;

EDENS_NAMESPACE_USING;

bool CUpdateIntervalSubsystem::Initialize(const FSubsystemInitializerBase* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		const FUpdateIntervalSubsystemInitializer* initializer = PCast<const FUpdateIntervalSubsystemInitializer*>(_Initializer);
		CHECK_RESULT_BREAK(initializer);

		CHECK_RESULT_BREAK(SetupTimer());

		CHECK_RESULT_BREAK(SetupIntervalParam(initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CUpdateIntervalSubsystem::Finalize()
{
	Delete(m_IntervalTimer);
}

bool CUpdateIntervalSubsystem::SetupTimer()
{
	m_IntervalTimer = new CTimer();
	CHECK_RESULT_FALSE(m_IntervalTimer);

	m_IntervalTimer->SetUp();

	m_IntervalTimer->RecordStartTime();

	return true;
}

bool CUpdateIntervalSubsystem::SetupIntervalParam(const FUpdateIntervalSubsystemInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_FramePerSecond = _Initializer->FramePerSecond;

	return true;
}

bool CUpdateIntervalSubsystem::Execution(const FUpdateFunction& _Func)
{
	bool result = false;

	m_IntervalTimer->RecordCurrentTime();

	const double FrameTime = 1.0 / m_FramePerSecond;

	double ExecuteTime = m_IntervalTimer->CalculateExecuteTime();
	m_ExecuteTime = ExecuteTime;
	if (ExecuteTime >= FrameTime)
	{
		//m_IntervalTimer->RecordStartTime();
		//m_IntervalTimer->RecordCurrentTime();

		m_IntervalTimer->RecordStartTime();

		_Func(SCast<float>(ExecuteTime));

		result = true;
	}

	//m_IntervalTimer->RecordCurrentTime();

	return result;
}
