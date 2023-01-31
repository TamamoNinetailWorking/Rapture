#pragma once

#include <Atlantis/DirectX12/DirectXPaste.h>
#include <vector>

ATLANTIS_NAMESPACE_BEGIN

struct FRootParameterInitializer
{
public:

	void PushParameter(Glue::EDescriptorHeapRangeType _Type, Glue::EShaderVisibility);

	FRootParameterInitializer();
	~FRootParameterInitializer();

private:

	void Reset();

private:

	friend class CRootParamBuilder;
	//friend class CRootParamBuilder::Impl;

	struct FRootParameterData
	{
		Glue::EDescriptorHeapRangeType RangeType = {};
		Glue::EShaderVisibility ShaderVisibility = {};
	};

	std::vector<FRootParameterData> m_RootParameter = {};
	
};


ATLANTIS_NAMESPACE_END