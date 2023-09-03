#pragma once

#include <list>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

MAGALLANICA_NAMESPACE_BEGIN

struct FDebugWindowSubsystemInitializer;

class CDebugWindow;

class CDebugWindowSubsystem
{
public:

	bool Initialize(const FDebugWindowSubsystemInitializer* _Initializer);
	void Finalize();


	void ShowDebugWindow(float _DeltaTime);
	void RenderDebugWindow();

	CDebugWindowSubsystem() {};
	~CDebugWindowSubsystem() {};

public:

	uint32 SetSRV(ID3D12Resource* _Resource);

	uint64 GetSRVGPUHandle(uint32 _Index) const;

protected:

	virtual bool InitializeImpl();

	void RegistDebugWindow(CDebugWindow* _DebugWindow);

private:

	void ShowDemoWindow();

	void ShowWindowImpl();

	void RegisteredFinalize();

private:

	using FDebugWindowList = std::list<CDebugWindow*>;

	FDebugWindowList m_DebugWindowList = {};

private:

	ID3D12DescriptorHeap* m_SRVDesc = nullptr;

	uint32 m_SrvCurrentNum = 0;

	uint32 m_HandleIncrementSize = 0;

	bool m_ShowDemoWindow = false;
	bool m_ShowAnotherWindow = false;

	bool m_ShowDebugCoreWindow = true;

};

MAGALLANICA_NAMESPACE_END