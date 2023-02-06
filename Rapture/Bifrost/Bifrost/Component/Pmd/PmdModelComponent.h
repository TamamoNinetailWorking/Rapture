#pragma once

#include <Bifrost/Component/RenderingComponent.h>

EDENS_NAMESPACE_BEGIN

class CFileLoader;

EDENS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CPmdParser;

struct FComponentInitializerBase;
struct FPmdModelComponentInitializer;

struct FSceneData;

class CPmdModelComponent : public CRenderingComponent
{
public:

	bool Initialize(const FComponentInitializerBase* _Initializer) override;
	void Finalize() override;

	void BeginPlay() override;
	void EndPlay() override;

	bool Update(float _DeltaTime) override;

	CPmdModelComponent();
	~CPmdModelComponent();

private:

	using Super = CRenderingComponent;

private:

	ObjectPtr(EDENS_NAMESPACE::CFileLoader) m_FileLoader = nullptr;
	ObjectPtr(CPmdParser) m_PmdParser = nullptr;

private:

	void FinalizeMesh();
	void FinalizeMaterial();

	bool ReadyPmdData(const FPmdModelComponentInitializer* _Initializer);

	bool FileLoad(const FPmdModelComponentInitializer* _Initializer);
	bool SearchMeshData(const FPmdModelComponentInitializer* _Initializer);
	bool CreateMeshData(const FPmdModelComponentInitializer* _Initializer);
	//bool SearchMaterial(const FPmdModelComponentInitializer* _Initializer);
	bool CreateMaterial(const FPmdModelComponentInitializer* _Initializer);
	void CloseFile();

private:

	bool SetTransform();

};


BIFROST_NAMESPACE_END