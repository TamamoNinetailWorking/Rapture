#pragma once

#include <eden/include/Resource/ResourceManagerPreDefine.h>

ATLANTIS_NAMESPACE_BEGIN

class CRenderTargetViewResource;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

struct FRenderTargetInitializer;

class CRenderTarget
{
public:

	bool Initialize(const FRenderTargetInitializer* _Initializer);
	void Finalize();

	const ATLANTIS_NAMESPACE::CRenderTargetViewResource* GetRenderTargetView() const;

	CRenderTarget() {};
	~CRenderTarget() {};

private:

	EDENS_NAMESPACE::FResourceHandle m_TextureHandle = {};
	EDENS_NAMESPACE::FResourceHandle m_RTVHandle = {};

private:

	bool CreateRenderTargetView(const FRenderTargetInitializer* _Initializer);

};

BIFROST_NAMESPACE_END