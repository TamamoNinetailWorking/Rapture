#pragma once

#include "RenderGroupDefine.h"
#include <Bifrost/Subsystem/SubsystemBase/SubsystemBase.h>

#include <Atlantis/DirectX12/DirectXPaste.h>

ATLANTIS_NAMESPACE_BEGIN

class CRHIProcessor;
class CRHIRenderTargetView;
class CSceneView;
class IMaterialInterface;

ATLANTIS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

class CRenderingQueue;

class CRenderingComponent;

struct FSubsystemInitializerBase;
struct FRenderingSubsystemInitializer;

class CMeshData;

class CRenderingSubsystem : public CSubsystemBase
{
public:

	using Processor = ATLANTIS_NAMESPACE::CRHIProcessor;
	using RTVStruct = ATLANTIS_NAMESPACE::CRHIRenderTargetView;
	using SceneView = ATLANTIS_NAMESPACE::CSceneView;
	using RenderingQueue = BIFROST_NAMESPACE::CRenderingQueue;

public:

	bool Initialize(const FSubsystemInitializerBase* _Initializer);
	void Finalize();

	void SetRenderObject(ERenderGroup _Group,const CRenderingComponent* _Component);

	void ClearRenderGroup();

	bool RenderBegin();
	void Rendering();
	void RenderEnd();

	CRenderingSubsystem();
	~CRenderingSubsystem();

public:

	const Processor* GetProcessor() const;
	Processor* GetProcessorEdit() const;

	const RTVStruct* GetRTV() const;
	RTVStruct* GetRTVEdit() const;

	const SceneView* GetSceneView() const;
	SceneView* GetSceneViewEdit() const;


public:

	bool SetPrimitiveTopology(ATLANTIS_NAMESPACE::Glue::EPrimitiveTopology _Topology);
	bool SetMeshData(const CMeshData* _Mesh);
	bool SetMaterialInterface(const ATLANTIS_NAMESPACE::IMaterialInterface* _Material);
	bool SetGraphicsRootDescriptorTable(uint32 _Offset, uint64 _HeapHandle);
	uint64 GetMaterialHeapHandle(const ATLANTIS_NAMESPACE::IMaterialInterface* _Material);
	bool DrawIndexedInstanced(uint32 _CurrentIndex, uint32 _IndexOffset);

private:

	void SortQueue();
	void RenderQueue(const RenderingQueue* _Queue);

private:

	ObjectPtr(Processor) m_Processor = nullptr;
	ObjectPtr(RTVStruct) m_RTV = nullptr;
	ObjectPtr(SceneView) m_SceneView = nullptr;

	using RenderGroup = std::array<ObjectPtr(RenderingQueue), UNumCast(ERenderGroup::RENDER_GROUP_NUM)>;
	RenderGroup m_RenderGroup = {};

private:

	bool CreateProcessor(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateRenderingQueue(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateRTV(const FRenderingSubsystemInitializer* _Initializer);
	bool CreateScreenView(const FRenderingSubsystemInitializer* _Initializer);

};

BIFROST_NAMESPACE_END