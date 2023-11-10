#include "SceneView.h"
#include "SceneViewInitializer.h"

#include <eden/include/utility/ender_utility.h>

#include <Atlantis/DirectX12/Viewport/Viewport.h>
#include <Atlantis/DirectX12/ScissorRect/ScissorRect.h>

#include <Atlantis/Math/MathLibrary.h>

USING_ATLANTIS;

//using namespace DirectX;

bool ATLANTIS_NAMESPACE::CSceneView::Initialize(const FSceneViewInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(CreateViewport(_Initializer));
		CHECK_RESULT_BREAK(CreateScissorRect(_Initializer));

		SetupProjectionMatrix(_Initializer);

		return true;
	} while (0);

	Finalize();
	return false;
}

void ATLANTIS_NAMESPACE::CSceneView::Finalize()
{
	EDENS_NAMESPACE::FinalizeObject(m_ScissorRect);
	EDENS_NAMESPACE::FinalizeObject(m_Viewport);
}

bool ATLANTIS_NAMESPACE::CSceneView::CreateViewport(const FSceneViewInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_Viewport = new CViewport();
	CHECK_RESULT_FALSE(m_Viewport);

	CViewport::FViewportInitializer initializer = {};
	initializer.ViewportWidth = _Initializer->ViewportWidth;
	initializer.VIewportHeight = _Initializer->ViewportHeight;

	return m_Viewport->Initialize(initializer);
}

bool ATLANTIS_NAMESPACE::CSceneView::CreateScissorRect(const FSceneViewInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	m_ScissorRect = new CScissorRect();
	CHECK_RESULT_FALSE(m_ScissorRect);

	CScissorRect::FInitializer initializer = {};
	initializer.RectWidth = SCast<uint32>(_Initializer->ViewportWidth);
	initializer.RectHeight = SCast<uint32>(_Initializer->ViewportHeight);

	return m_ScissorRect->Initialize(initializer);
}

void ATLANTIS_NAMESPACE::CSceneView::SetupProjectionMatrix(const FSceneViewInitializer* _Initializer)
{
	CHECK(_Initializer);

	float aspectRatio = _Initializer->ViewportWidth / _Initializer->ViewportHeight;

	m_ProjectionMatrix = FMathLibrary::MatrixPerspectiveFovLH(
		_Initializer->FovAngle,
		aspectRatio,
		_Initializer->NearZ,
		_Initializer->FarZ
	);

}
