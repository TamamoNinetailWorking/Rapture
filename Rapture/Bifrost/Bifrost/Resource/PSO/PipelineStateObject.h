#pragma once

#include <eden/include/Resource/Resource.h>

#include <eden/include/Resource/ResourceManagerPreDefine.h>

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

class CGraphicsPipeline;
class CRootSignature;

ATLANTIS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

struct FPipelineStateObjectInitializer;

class CPipelineStateObject : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	ATLANTIS_NAMESPACE::CGraphicsPipeline* GetPipeline() const { return m_Pipeline; };
	ATLANTIS_NAMESPACE::CRootSignature* GetRootSignature() const { return m_RootSignature; };

private:

	bool CreatePipeline(const FPipelineStateObjectInitializer* _Initializer);
	bool CreateRootSignature(const FPipelineStateObjectInitializer* _Initializer);

	void ReleaseShader();

	ObjectPtr(ATLANTIS_NAMESPACE::CGraphicsPipeline) m_Pipeline = nullptr;
	ObjectPtr(ATLANTIS_NAMESPACE::CRootSignature) m_RootSignature = nullptr;

	EDENS_NAMESPACE::FResourceHandle m_VSHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_HSHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_DSHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_GSHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_PSHandle = {};

};

ATLANTIS_NAMESPACE_END