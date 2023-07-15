#pragma once

#include <array>

#include <Bifrost/Component/Rendering/RenderingComponent.h>

BIFROST_NAMESPACE_BEGIN

class CRenderTarget;
class CMeshData;

struct FBloomPostEffectComponentInitializer;

constexpr uint8 RTV_BUFFER_NUM = 5;

class CBloomEffectComponent : public CRenderingComponent
{
private:
	using Super = CRenderingComponent;
public:

	bool Initialize(const FComponentInitializerBase* _Initializer) override;
	void Finalize() override;

	bool Draw() const override;

	CBloomEffectComponent();

private:

	typedef std::array<CRenderTarget*, RTV_BUFFER_NUM> RTVBuffers;
	RTVBuffers m_RTVBuffers = {};

private:

	bool CreateRTVBuffers();
	bool CreateMeshData();
	bool CreateMaterial(const FBloomPostEffectComponentInitializer* _Initializer);


public:

	const RTVBuffers& GetRTVBuffers() const { return m_RTVBuffers; };
	const CRenderTarget* GetRTV(uint8 _Index) const { return m_RTVBuffers.at(_Index); };

};

BIFROST_NAMESPACE_END