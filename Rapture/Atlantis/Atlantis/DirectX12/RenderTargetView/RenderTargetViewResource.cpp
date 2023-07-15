#include "RenderTargetViewResource.h"
#include "RenderTargetViewResourceInitializer.h"
#include "RenderTargetView.h"

#include <eden/include/utility/ender_utility.h>

USING_ATLANTIS;

bool ATLANTIS_NAMESPACE::CRenderTargetViewResource::Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _initializer)
{
	do
	{
		const FRenderTargetViewResourceInitializer* initializer = PCast<const FRenderTargetViewResourceInitializer*>(_initializer);

		CHECK_RESULT_BREAK(CreateRenderTargetView(initializer));

		m_ResourceName = initializer->Name;

		return true;
	} while (0);

	Finalize();
	return false;
}

void ATLANTIS_NAMESPACE::CRenderTargetViewResource::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_RenderTargetView);
}

bool ATLANTIS_NAMESPACE::CRenderTargetViewResource::CreateRenderTargetView(const FRenderTargetViewResourceInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_RenderTargetView = new CRenderTargetView();
	CHECK_RESULT_FALSE(m_RenderTargetView);

	CRenderTargetView::FRenderTargetViewInitializer initializer = {};
	initializer.Device = _Initializer->Device;
	initializer.ResPtr = _Initializer->ResPtr;
	initializer.Format = _Initializer->Format;
	initializer.RTVDimension = _Initializer->RTVDimension;

	return m_RenderTargetView->Initialize(initializer);
}
