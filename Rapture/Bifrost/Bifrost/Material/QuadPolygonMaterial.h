#pragma once

#include <Atlantis/Material/MaterialInterface.h>
#include <eden/include/Resource/ResourceManagerPreDefine.h>

struct ID3D12Resource;

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialInterfaceInitializerBase;
struct FMaterialGeometryBufferBase;
class CGraphicsPipeline;
class CRootSignature;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CPipelineStateObject;
struct FQuadPolygonMaterialInitializer;
struct FQuadPolygonSceneData;

class CQuadPolygonMaterial : public ATLANTIS_NAMESPACE::IMaterialInterface
{
private:
	using Super = ATLANTIS_NAMESPACE::IMaterialInterface;
public:

	bool Initialize(const ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase* _Initializer) override;

	void Finalize() override;

	void SetGeometryBuffer(ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* _Buffer) override;

	uint32 GetDrawIndex(uint32 _Index) const override;
	uint32 GetMaterialNum() const override;

	ATLANTIS_NAMESPACE::CGraphicsPipeline* GetGraphicsPipeline() const override;
	ATLANTIS_NAMESPACE::CRootSignature* GetRootSignature() const override;

	const ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBuffer() const override;
	ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBufferEdit() const override;

	CQuadPolygonMaterial();
	~CQuadPolygonMaterial();

private:

	ObjectPtr(ID3D12Resource) m_VertexConstantBuffer = nullptr;
	ObjectPtr(FQuadPolygonSceneData) m_SceneData = nullptr;

	EDENS_NAMESPACE::FResourceHandle m_MaterialHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_PsoHandle = {};

private:

	const CPipelineStateObject* GetPipelineStateObject() const;

	bool CreateMaterials(const FQuadPolygonMaterialInitializer* _Initializer);
	bool CreatePipelineStateObject(const FQuadPolygonMaterialInitializer* _Initializer);
	bool CreateSceneData(const FQuadPolygonMaterialInitializer* _Initializer);

};


BIFROST_NAMESPACE_END