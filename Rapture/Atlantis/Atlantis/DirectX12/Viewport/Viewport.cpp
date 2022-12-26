#include "Viewport.h"

#include <eden/include/utility/ender_utility.h>

EDENS_NAMESPACE_USING;
USING_ATLANTIS;

bool CViewport::Initialize(const FViewportInitializer& _Initializer)
{
	do
	{
		if (!CreateViewport(_Initializer)) { break; }

		return true;
	} while (0);

	return false;
}

void CViewport::Finalize()
{
	if (m_Viewport)
	{
		delete m_Viewport;
		m_Viewport = nullptr;
		//m_Viewport.reset();
	}
}

bool CViewport::CreateViewport(const FViewportInitializer& _Initializer)
{
	D3D12_VIEWPORT* viewport = new D3D12_VIEWPORT();
	if (!viewport) { return false; }

	viewport->Width = _Initializer.ViewportWidth;
	viewport->Height = _Initializer.VIewportHeight;
	viewport->TopLeftX = 0.f;
	viewport->TopLeftY = 0.f;
	viewport->MaxDepth = 1.f;
	viewport->MinDepth = 0.f;

	//m_Viewport.reset(viewport);
	m_Viewport = viewport;

	return true;
}
