#pragma once

#include <eden/include/Resource/Resource.h>

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

ATLANTIS_NAMESPACE_BEGIN

struct FRenderTargetViewResourceInitializer;

class CRenderTargetView;

class CRenderTargetViewResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _initializer) override;
	void Finalize() override;

	CRenderTargetView* GetRenderTargetView() const { return m_RenderTargetView; };

private:

	ObjectPtr(CRenderTargetView) m_RenderTargetView = nullptr;

private:

	bool CreateRenderTargetView(const FRenderTargetViewResourceInitializer* _Initializer);

};

ATLANTIS_NAMESPACE_END