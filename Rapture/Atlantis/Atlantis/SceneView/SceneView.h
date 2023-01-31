#pragma once

#include <Atlantis/DirectX12/GlueMath.h>

ATLANTIS_NAMESPACE_BEGIN

class CViewport;
class CScissorRect;

struct FSceneViewInitializer;

class CSceneView
{
public:

	bool Initialize(const FSceneViewInitializer* _Initializer);
	void Finalize();

	const Glue::FMatrix& GetProjectionMatrix() const { return m_ProjectionMatrix; };
	const CViewport* GetViewport() const { return m_Viewport; };
	const CScissorRect* GetScissorRect() const { return m_ScissorRect; };

	CSceneView() {};
	~CSceneView() {};

private:

	Glue::FMatrix m_ProjectionMatrix = {};

	ObjectPtr(CViewport) m_Viewport = nullptr;
	ObjectPtr(CScissorRect) m_ScissorRect = nullptr;

private:

	bool CreateViewport(const FSceneViewInitializer* _Initializer);
	bool CreateScissorRect(const FSceneViewInitializer* _Initializer);
	void SetupProjectionMatrix(const FSceneViewInitializer* _Initializer);

};

ATLANTIS_NAMESPACE_END