﻿#pragma once

namespace Test 
{
	void TestMain();
};




ATLANTIS_NAMESPACE_BEGIN
class CDX12MainDevice;
ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CMeshData;
class CPmdMaterialData;
class CPipelineStateObject;

class CPmdModelComponent;
class CTransformComponent;

class CLightActor;
class CCameraActor;

BIFROST_NAMESPACE_END

namespace Test
{
	void SHA1TestMain();

	void MatrixVectorTestMain();

	void FileLoaderTest(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device);

	void GraphicsPipelineTest(ATLANTIS_NAMESPACE::CDX12MainDevice* _Device);

	void BindFunctionTest();

	void RenderSubsystemTest();

	extern BIFROST_NAMESPACE::CPipelineStateObject* m_Pso;
	//extern ATLANTIS_NAMESPACE::CVertexBuffer* m_VertexBuffer;
	//extern ATLANTIS_NAMESPACE::CIndexBuffer* m_IndexBuffer;
	extern BIFROST_NAMESPACE::CMeshData* m_MeshData;
	extern BIFROST_NAMESPACE::CPmdMaterialData* m_MaterialData;

	//extern BIFROST_NAMESPACE::CPmdModelComponent* m_PmdModelComponent;
#if 0
	extern BIFROST_NAMESPACE::CTransformComponent* m_TransformComponent;

	extern BIFROST_NAMESPACE::CLightActor* m_LightActor;
	extern BIFROST_NAMESPACE::CCameraActor* m_CameraActor;
#endif
};