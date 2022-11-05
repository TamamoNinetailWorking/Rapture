#pragma once

#include <memory>

#include <d3d12.h>

ATLANTIS_NAMESPACE_BEGIN

class CViewport
{
public:

	struct FViewportInitializer
	{
		float ViewportWidth = 0.f;
		float VIewportHeight = 0.f;
	};

	bool Initialize(const FViewportInitializer& _Initializer);

	void Finalize();

	const D3D12_VIEWPORT* GetViewport() const { return m_Viewport.get(); };

	CViewport() {};
	~CViewport() {};

private:

	bool CreateViewport(const FViewportInitializer& _Initializer);

	std::unique_ptr<D3D12_VIEWPORT> m_Viewport = nullptr;

};

ATLANTIS_NAMESPACE_END