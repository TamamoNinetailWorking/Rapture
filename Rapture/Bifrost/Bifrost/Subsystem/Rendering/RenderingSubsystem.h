#pragma once

#include "RenderGroupDefine.h"

ATLANTIS_NAMESPACE_BEGIN

class CRHIProcessor;
class CRHIRenderTargetView;
class CSceneView;

ATLANTIS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

class CRenderingQueue;

class CRenderingComponent;

struct FRenderingSubsystemInitializer;

class CRenderingSubsystem
{
public:

	using Processor = ATLANTIS_NAMESPACE::CRHIProcessor;
	using RTVStruct = ATLANTIS_NAMESPACE::CRHIRenderTargetView;
	using SceneView = ATLANTIS_NAMESPACE::CSceneView;
	using RenderingQueue = BIFROST_NAMESPACE::CRenderingQueue;


	bool Initialize(const FRenderingSubsystemInitializer* _Initializer);
	void Finalize();

	void SetRenderObject(ERenderGroup _Group,const CRenderingComponent* _Component);

	void ClearRenderGroup();

	void Rendering();

	
	const Processor* GetProcessor() const;
	Processor* GetProcessorEdit() const;

	const RTVStruct* GetRTV() const;
	RTVStruct* GetRTVEdit() const;

	const SceneView* GetSceneView() const;
	SceneView* GetSceneViewEdit() const;


	CRenderingSubsystem();
	~CRenderingSubsystem();

private:

	void SortQueue();
	void RenderQueue(const RenderingQueue* _Queue);

private:

	ObjectPtr(Processor) m_Processor = nullptr;
	ObjectPtr(RTVStruct) m_RTV = nullptr;
	ObjectPtr(SceneView) m_SceneView = nullptr;

	using RenderGroup = std::array<ObjectPtr(RenderingQueue), ERenderGroup::RENDER_GROUP_NUM>;
	RenderGroup m_RenderGroup = {};

private:

	bool CreateProcessor(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateRenderingQueue(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateRTV(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateScreenView(const FRenderingSubsystemInitializer* _Initializer);

};

BIFROST_NAMESPACE_END