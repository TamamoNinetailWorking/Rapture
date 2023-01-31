#include "RootParamBuilder.h"
#include "RootSignatureInitializer.h"

#include <vector>
#include <map>

#include <d3d12.h>

USING_ATLANTIS;

using namespace Glue;

class CRootParamBuilder::Impl
{
public:

	void SetupRootParam(const FRootParameterInitializer* _Initializer);

	void SetupDescritproRange(const FRootParameterInitializer* _Initializer);

	void Setup(const FRootParameterInitializer* _Initializer);
	
	void Reset();

private:



	//std::vector<D3D12_DESCRIPTOR_RANGE> m_Ranges = {};
	//std::vector<D3D12_ROOT_PARAMETER> m_Parameters = {};

	// RootParamはタイプ別でデータを持つ方がいいだろう
	// データを持ち直してやり直した方がきっとすっきり終わるはず
	// mapでやるとレジスター位置がずれる気がする・・・
	std::vector<D3D12_DESCRIPTOR_RANGE> m_Ranges = {};
	std::vector<D3D12_ROOT_PARAMETER> m_Parameters = {};
};

CRootParamBuilder::CRootParamBuilder()
{
	m_Impl = new Impl();
}

CRootParamBuilder::~CRootParamBuilder()
{
}


bool CRootParamBuilder::Initialize(const FRootParameterInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_FALSE(_Initializer);

		m_Impl->SetupRootParam(_Initializer);
		m_Impl->SetupDescritproRange(_Initializer);

		return true;
	} while (0);

	Finalize();
	return false;
}

void CRootParamBuilder::Finalize()
{
	m_Impl->Reset();
}

void CRootParamBuilder::Impl::Setup(const FRootParameterInitializer* _Initializer)
{
	CHECK(_Initializer);

	m_Parameters.reserve(_Initializer->m_RootParameter.size());
	m_Ranges.reserve(_Initializer->m_RootParameter.size());

	for (auto& elem : _Initializer->m_RootParameter)
	{
		EShaderVisibility visibility = elem.ShaderVisibility;

		{
			bool isExist = false;
			for (auto& data : m_Parameters)
			{
				if (data.ShaderVisibility != GetShaderVisibility(visibility))
				{
					continue;
				}
				++data.DescriptorTable.NumDescriptorRanges;
				isExist = true;
			}

			if (!isExist)
			{

			}
		}

		{
			bool isExist = false;
			for (auto& data : m_Ranges)
			{

			}
		}

	}
}

#if 0
void CRootParamBuilder::Impl::SetupRootParam(const FRootParameterInitializer* _Initializer)
{
	CHECK(_Initializer);

	m_Parameters.reserve(_Initializer->m_RootParameter.size());

	for (auto& element : _Initializer->m_RootParameter)
	{
		Glue::EShaderVisibility shaderVisibility = element.ShaderVisibility;
		D3D12_ROOT_PARAMETER rootParam = {};
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParam.ShaderVisibility = Glue::GetShaderVisibility(shaderVisibility);
		
		bool isExist = false;
		for (auto& param : m_Parameters)
		{
			if (rootParam.ShaderVisibility == param.ShaderVisibility)
			{
				++param.DescriptorTable.NumDescriptorRanges;
				isExist = true;
				break;
			}
		}

		if (!isExist)
		{
			rootParam.DescriptorTable.NumDescriptorRanges = 1;
			m_Parameters.push_back(rootParam);
		}

	}
}

void CRootParamBuilder::Impl::SetupDescritproRange(const FRootParameterInitializer* _Initializer)
{
	CHECK(_Initializer);

	m_Ranges.reserve(_Initializer->m_RootParameter.size());

#if 0

	uint32 rangeIndex = 0;
	for (uint32 i = 0; i < m_Parameters.size(); ++i)
	{
		auto& srcData = m_Parameters.at(i);
		rangeIndex += srcData.DescriptorTable.NumDescriptorRanges;
		// ルートパラメータのVisibility順にRangeType別に仕分けしていく
		for (auto& elem : _Initializer->m_RootParameter)
		{
			Glue::EShaderVisibility visibility = elem.ShaderVisibility;

			if (srcData.ShaderVisibility != Glue::GetShaderVisibility(visibility))
			{
				continue;
			}

			// ここはDescirptorsの数をカウントする
			// いくつ目までがRootParameterに対応しているのかが必要
			// >>それがNumDescriptorRangesに対応しているはず
			bool isExist = false;
			//uint32 index = 0;
			//for (uint32 k = rangeIndex - 1; k < m_Ranges.size(); ++k)
			Glue::EDescriptorHeapRangeType type = elem.RangeType;
			uint32 shaderRegister = 0;
			for (uint32 k = 0; k < m_Ranges.size(); ++k)
			{
				auto& data = m_Ranges.at(k);

				if (data.RangeType != Glue::GetRangeType(type))
				{
					continue;
				}

				if (!isExist)
				{
					if (k >= (rangeIndex - 1))
					{
						++data.NumDescriptors;
						isExist = true;
					}
					//index = k;
				}
				else
				{
					++data.BaseShaderRegister;
				}

				shaderRegister += data.NumDescriptors;

				//break;
			}

			if (isExist)
			{
				continue;

			}

#if 1
			D3D12_DESCRIPTOR_RANGE range = {};
			range.NumDescriptors = 1;
			range.RangeType = Glue::GetRangeType(elem.RangeType);
			range.BaseShaderRegister = shaderRegister;
			range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			m_Ranges.push_back(range);

			uint32 position = 0;
			position = SCast<uint32>(m_Ranges.size() - 1);

			srcData.DescriptorTable.pDescriptorRanges = &m_Ranges[position];
#else 

			uint32 position = 0;
			position = SCast<uint32>(m_Ranges.size() - 1);

			srcData.DescriptorTable.pDescriptorRanges = &m_Ranges[position];
			
			{
				D3D12_DESCRIPTOR_RANGE range = {};
				range.NumDescriptors = 1;
				range.RangeType = Glue::GetRangeType(elem.RangeType);
				range.BaseShaderRegister = 0;
				range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


				for (uint32 j = 0; j < index; ++j)
				{
					auto& data = m_Ranges.at(j);
					if (data.RangeType != Glue::GetRangeType(elem.RangeType))
					{
						continue;
					}

					range.BaseShaderRegister += data.NumDescriptors;
					//position = j;
				}

				m_Ranges.push_back(range);
				
				uint32 position = 0;
				position = SCast<uint32>(m_Ranges.size() - 1);
				
				srcData.DescriptorTable.pDescriptorRanges = &m_Ranges[position];
			}
#endif

		}
	}

#else

	for (auto& elem : _Initializer->m_RootParameter)
	{
		uint32 rangeIndex = 0;
		uint32 rangeMax = 0;

		for (auto& param : m_Parameters)
		{
			rangeMax += param.DescriptorTable.NumDescriptorRanges;

			if (Glue::GetShaderVisibility(elem.ShaderVisibility) != param.ShaderVisibility)
			{
				rangeIndex += param.DescriptorTable.NumDescriptorRanges;
				continue;
			}

			uint32 shaderRegister = 0;
			bool isExist = false;
			for (uint32 i = 0; i < m_Ranges.size(); ++i)
			{
				auto& data = m_Ranges.at(i);

				if (Glue::GetRangeType(elem.RangeType) != data.RangeType)
				{
					continue;
				}

				if (!isExist)
				{
					if ((rangeIndex < i) && (i <= rangeMax))
					{
						++data.NumDescriptors;
						isExist = true;
					}
				}
				else
				{
					++data.BaseShaderRegister;
				}


				shaderRegister += data.NumDescriptors;

			}

			rangeIndex += param.DescriptorTable.NumDescriptorRanges;

			if (isExist)
			{
				break;

			}

			D3D12_DESCRIPTOR_RANGE range = {};
			range.NumDescriptors = 1;
			range.RangeType = Glue::GetRangeType(elem.RangeType);
			range.BaseShaderRegister = shaderRegister;
			range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

			m_Ranges.push_back(range);

			uint32 position = 0;
			position = SCast<uint32>(m_Ranges.size() - 1);

			param.DescriptorTable.pDescriptorRanges = &m_Ranges[position];
			break;
		}
	}


#endif
}

#endif

void CRootParamBuilder::Impl::Reset()
{
	m_Parameters.clear();
	m_Ranges.clear();
}
