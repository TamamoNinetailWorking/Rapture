#include "RootSignatureInitializer.h"

USING_ATLANTIS;

FRootParameterInitializer::FRootParameterInitializer()
{
	// åàÇﬂë≈ÇøÇ≈ç°èëÇ¢ÇƒÇÈ
	m_RootParameter.reserve(10);
}

FRootParameterInitializer::~FRootParameterInitializer()
{
	Reset();
}

void ATLANTIS_NAMESPACE::FRootParameterInitializer::Reset()
{
	m_RootParameter.resize(0);
}


void FRootParameterInitializer::PushParameter(Glue::EDescriptorHeapRangeType _Type, Glue::EShaderVisibility _Visibility)
{
	FRootParameterData data = {};
	data.RangeType = _Type;
	data.ShaderVisibility = _Visibility;

	m_RootParameter.push_back(data);
}