#pragma once

#include <array>

#include "BloomPostEffectComponentPreDefine.h"
#include <Bifrost/Component/Rendering/RenderingComponent.h>

ATLANTIS_NAMESPACE_BEGIN

class IMaterialInterface;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CRenderTarget;
class CMeshData;

struct FBloomPostEffectComponentInitializer;


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

	typedef std::array<CRenderTarget*, SCast<uint8>(EBloomPostEffectRTVBufferIndex::RTV_BUFFER_NUM)> RTVBuffers;
	RTVBuffers m_RTVBuffers = {};

private:

	bool CreateRTVBuffers();
	bool CreateMeshData();
	bool CreateMaterial(const FBloomPostEffectComponentInitializer* _Initializer);
	bool CreateParameterBuffer();

private:

	const FBloomPostEffectParameterBuffer CalcBlurParam(uint32 _Width, uint32 _Height, const ATLANTIS_NAMESPACE::Glue::Vector2& _Direction, float _Deviation);

public:

	const RTVBuffers& GetRTVBuffers() const { return m_RTVBuffers; };
	const CRenderTarget* GetRTV(uint8 _Index) const { return m_RTVBuffers.at(_Index); };

public:

	const CRenderTarget* GetResultRTV() const { return m_RTVBuffers.at(0); };

private:

	ATLANTIS_NAMESPACE::IMaterialInterface* m_BrightnessInterface = nullptr;
	ATLANTIS_NAMESPACE::IMaterialInterface* m_BlurInterface = nullptr;
	ATLANTIS_NAMESPACE::IMaterialInterface* m_FetchColorInterface = nullptr;

	ObjectPtr(FBloomPostEffectParameterBuffer) m_ParameterBuffer = nullptr;

};

BIFROST_NAMESPACE_END