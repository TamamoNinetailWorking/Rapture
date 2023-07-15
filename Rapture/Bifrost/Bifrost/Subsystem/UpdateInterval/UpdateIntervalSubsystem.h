#pragma once

#include <Bifrost/Subsystem/SubsystemBase/SubsystemBase.h>

EDENS_NAMESPACE_BEGIN

class CTimer;

EDENS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

struct FSubsystemInitializerBase;
struct FUpdateIntervalSubsystemInitializer;

class CUpdateIntervalSubsystem : public CSubsystemBase
{
public:

	bool Initialize(const FSubsystemInitializerBase* _Initializer);
	void Finalize();

	CUpdateIntervalSubsystem() {};
	~CUpdateIntervalSubsystem() {};

public:

	typedef std::function<void(float)> FUpdateFunction;

	bool Execution(const FUpdateFunction& _Func);

public:

	//double CalculateInterval() const;
	double GetExecuteTime() const { return m_ExecuteTime; };

private:

	bool SetupTimer();
	bool SetupIntervalParam(const FUpdateIntervalSubsystemInitializer* _Initializer);

private:

	EDENS_NAMESPACE::CTimer* m_IntervalTimer = nullptr;

private:

	double m_ExecuteTime = 0.0;
	uint16 m_FramePerSecond = 0;

};


BIFROST_NAMESPACE_END