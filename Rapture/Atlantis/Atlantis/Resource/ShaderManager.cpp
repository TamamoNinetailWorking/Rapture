#include "ShaderManager.h"

#include <Atlantis/DirectX12/Shader/ShaderBase.h>

USING_ATLANTIS;
EDENS_NAMESPACE_USING;

CResource* CShaderManager::CreateResourceObject()
{
	CShaderBase* ptr = new CShaderBase();
	return ptr;
}
