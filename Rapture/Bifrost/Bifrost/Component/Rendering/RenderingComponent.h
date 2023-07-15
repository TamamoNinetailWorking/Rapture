#pragma once

#include <Bifrost/Component/Component.h>
#include <Bifrost/Subsystem/Rendering/RenderGroupDefine.h>

ATLANTIS_NAMESPACE_BEGIN

class IMaterialInterface;


ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CMeshData; // Bifrostのデータを直で持ってるのが気になる！

class CRenderingComponent : public CComponent
{
public:

	virtual bool Initialize(const FComponentInitializerBase* _Initializer) override;
	virtual void Finalize() override;

	virtual void BeginPlay() override;
	virtual void EndPlay() override;

	virtual bool Update(float _DeltaTime) override;

	virtual bool Draw() const;

	const BIFROST_NAMESPACE::CMeshData* GetMeshData() const;
	const ATLANTIS_NAMESPACE::IMaterialInterface* GetMaterialInterface() const;


	ERenderGroup GetRenderGroup() const;

	CRenderingComponent();
	virtual ~CRenderingComponent();

protected:

	ObjectPtr(const BIFROST_NAMESPACE::CMeshData) m_MeshData = nullptr;
	ObjectPtr(ATLANTIS_NAMESPACE::IMaterialInterface) m_MaterialInterface = nullptr;

	ERenderGroup m_RenderGroup = ERenderGroup::RENDER_GROUP_INVALID;

private:

	using Super = CComponent;

	bool RegistRenderer();

};


BIFROST_NAMESPACE_END