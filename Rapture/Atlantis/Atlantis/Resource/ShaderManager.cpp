#include "ShaderManager.h"

#include <eden/include/Resource/ResourceInitializer.h>
#include <Atlantis/DirectX12/Shader/ShaderBase.h>
#include <Atlantis/DirectX12/Shader/ShaderBaseInitializer.h>
#include <Atlantis/DirectX12/Shader/GraphicsShader.h>
//#include <Atlantis/DirectX12/Shader/GraphicsShaderInitializer.h>

USING_ATLANTIS;
EDENS_NAMESPACE_USING;


FResourceHandle CShaderManager::CreateResourceImpl(const FResourceInitializerBase* _Initializer)
{
	const FShaderBaseInitializer* baseInit = PCast<const FShaderBaseInitializer*>(_Initializer);
	if (_Initializer == nullptr) { return GetInvalidHandle(); }

	switch (baseInit->Type)
	{
	case EShaderType::SHADER_TYPE_VERTEX:
		return CreateResource<CVertexShader>(_Initializer);
	
	case EShaderType::SHADER_TYPE_DOMAIN:
		return CreateResource<CDomainShader>(_Initializer);

	case EShaderType::SHADER_TYPE_HULL:
		return CreateResource<CHullShader>(_Initializer);

	case EShaderType::SHADER_TYPE_GEOMETRY:
		return CreateResource<CGeometryShader>(_Initializer);

	case EShaderType::SHADER_TYPE_PIXEL:
		return CreateResource<CPixelShader>(_Initializer);

	default:
		return GetInvalidHandle();
		break;
	}

	return CreateResource<CShaderBase>(_Initializer);
}
