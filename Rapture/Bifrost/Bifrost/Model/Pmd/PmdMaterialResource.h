#pragma once

#include <eden/include/Resource/Resource.h>
#include <eden/include/Resource/ResourceManagerPreDefine.h>

struct ID3D12Resource;

EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

struct FPmdMaterialResourceInitializer;

class CPmdMaterialResource : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	ID3D12Resource* GetConstantResource() const;
	const EDENS_NAMESPACE::FResourceHandle& GetDecalTextureHandle() const;
	const EDENS_NAMESPACE::FResourceHandle& GetSphereTextureHandle() const;
	const EDENS_NAMESPACE::FResourceHandle& GetSphereAddTextureHandle() const;
	const EDENS_NAMESPACE::FResourceHandle& GetToonTextureHandle() const;

	CPmdMaterialResource();
	~CPmdMaterialResource();

private:

	ObjectPtr(ID3D12Resource) m_ConstantBuffer = nullptr;
	
	typedef EDENS_NAMESPACE::FResourceHandle Handle;

	Handle m_DecalTexture = {};
	Handle m_SphereTexture = {};
	Handle m_SphereAddTexture = {};
	Handle m_ToonTexture = {};

private:

	bool CreateConstantBuffer(const FPmdMaterialResourceInitializer* _Initializer);
	bool CreateMaterialTexture(const FPmdMaterialResourceInitializer* _Initializer);

};

BIFROST_NAMESPACE_END