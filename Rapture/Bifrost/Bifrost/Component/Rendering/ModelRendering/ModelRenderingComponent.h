#pragma once

#include <Bifrost/Component/Rendering/RenderingComponent.h>

BIFROST_NAMESPACE_BEGIN

class CModelRenderingComponent : public CRenderingComponent
{
private:

	using Super = CRenderingComponent;

public:

	CModelRenderingComponent();

public:

	bool Draw() const override;
};

BIFROST_NAMESPACE_END