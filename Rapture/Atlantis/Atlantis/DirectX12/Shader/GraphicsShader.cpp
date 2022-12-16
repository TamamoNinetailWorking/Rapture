#include "GraphicsShader.h"

#include <functional>
#include <d3dcompiler.h>

#include <Atlantis/DirectX12/DirectX12BaseDefine.h>

using namespace std;
USING_ATLANTIS;

CGraphicsShader::~CGraphicsShader()
{
	CShaderBase::~CShaderBase();
}


bool CVertexShader::Initialize(const FInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	FInitializerBase* notConstant = CCast<FInitializerBase*>(_Initializer);
	TargetSet(PCast<FInitializerMiddle*>(notConstant)) = CHash160("vs_5_0");
	CHECK_RESULT_FALSE(CShaderBase::Initialize(_Initializer));
	CHECK_RESULT_FALSE(CreateReflection(PCast<const FInitializer*>(_Initializer)));
	CHECK_RESULT_FALSE(CreateInputLayout(PCast<const FInitializer*>(_Initializer)));

	return true;
}

void CVertexShader::Finalize()
{
	// To DO
	ReleaseD3DPtr(m_Reflection);
	CShaderBase::Finalize();
}

CVertexShader::~CVertexShader()
{
	CGraphicsShader::~CGraphicsShader();
}

bool CVertexShader::CreateReflection(const FInitializer* _Initializer)
{
	HRESULT result = S_OK;

	ID3D12ShaderReflection* reflection = nullptr;
	D3D_ERROR_CHECK(D3DReflect(
		m_ShaderBlob->GetBufferPointer(),
		m_ShaderBlob->GetBufferSize(),
		IID_PPV_ARGS(&reflection)));

	m_Reflection.reset(reflection);

	return true;
}

bool CVertexShader::CreateInputLayout(const FInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(m_Reflection);
	CHECK_RESULT_FALSE(_Initializer->Device);

	D3D12_SHADER_DESC shaderDesc;
	D3D_ERROR_CHECK(m_Reflection->GetDesc(&shaderDesc));

	typedef function < DXGI_FORMAT(D3D_REGISTER_COMPONENT_TYPE, BYTE,D3D_MIN_PRECISION)> GetFormat;

	static const GetFormat GetDxgiFormat = [](D3D_REGISTER_COMPONENT_TYPE type, BYTE mask,D3D_MIN_PRECISION precision)
	{
		if (mask == 0x0F) // 1111
		{
			//xyzw
			switch (type)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			case D3D_REGISTER_COMPONENT_UINT32:
				if (precision == D3D_MIN_PRECISION_UINT_16)
				{
					return DXGI_FORMAT_R16G16B16A16_UINT;
				}
				else
				{
					return DXGI_FORMAT_R32G32B32A32_UINT;
				}
			case D3D_REGISTER_COMPONENT_SINT32:
				if (precision == D3D_MIN_PRECISION_SINT_16)
				{
					return DXGI_FORMAT_R16G16B16A16_SINT;
				}
				else
				{
					return DXGI_FORMAT_R32G32B32A32_SINT;
				}
			case D3D_REGISTER_COMPONENT_FLOAT32:
				if (precision == D3D_MIN_PRECISION_FLOAT_16)
				{
					return DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				else
				{
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
				}
			}
		}

		if (mask == 0x07) // 0111
		{
			//xyz
			switch (type)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				return DXGI_FORMAT_R32G32B32_TYPELESS;
			case D3D_REGISTER_COMPONENT_UINT32:
				return DXGI_FORMAT_R32G32B32_UINT;
			case D3D_REGISTER_COMPONENT_SINT32:
				return DXGI_FORMAT_R32G32B32_SINT;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				return DXGI_FORMAT_R32G32B32_FLOAT;
			}
		}

		if (mask == 0x03) // 0011
		{
			//xy
			switch (type)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				return DXGI_FORMAT_R32G32_TYPELESS;
			case D3D_REGISTER_COMPONENT_UINT32:
				if (precision == D3D_MIN_PRECISION_UINT_16)
				{
					return DXGI_FORMAT_R16G16_UINT;
				}
				else
				{
					return DXGI_FORMAT_R32G32_UINT;
				}
			case D3D_REGISTER_COMPONENT_SINT32:
				if (precision == D3D_MIN_PRECISION_SINT_16)
				{
					return DXGI_FORMAT_R16G16_SINT;
				}
				else
				{
					return DXGI_FORMAT_R32G32_SINT;
				}
			case D3D_REGISTER_COMPONENT_FLOAT32:
				if (precision == D3D_MIN_PRECISION_FLOAT_16)
				{
					return DXGI_FORMAT_R16G16_FLOAT;
				}
				else
				{
					return DXGI_FORMAT_R32G32_FLOAT;
				}

			}
		}

		if (mask == 0x01) // 0001
		{
			//x
			switch (type)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				return DXGI_FORMAT_R32_TYPELESS;
			case D3D_REGISTER_COMPONENT_UINT32:
				if (precision == D3D_MIN_PRECISION_UINT_16)
				{
					//return DXGI_FORMAT_R8_UINT; // 頂点データの指定でuint8を指定されたものの、GPU内で8bit変数定義ができない！！！
					return DXGI_FORMAT_R16_UINT;
				}
				else
				{
					return DXGI_FORMAT_R32_UINT;
				}
			case D3D_REGISTER_COMPONENT_SINT32:
				if (precision == D3D_MIN_PRECISION_SINT_16)
				{
					return DXGI_FORMAT_R16_SINT;
				}
				else
				{
					return DXGI_FORMAT_R32_SINT;
				}
			case D3D_REGISTER_COMPONENT_FLOAT32:
				if (precision == D3D_MIN_PRECISION_FLOAT_16)
				{
					return DXGI_FORMAT_R16_FLOAT;
				}
				else
				{
					return DXGI_FORMAT_R32_FLOAT;
				}
			}
		}

		return DXGI_FORMAT_UNKNOWN;
	};

	uint32 parameterNum = shaderDesc.InputParameters;
	InputLayout* input = new InputLayout();
	CHECK_RESULT_FALSE(input);
	input->reserve(parameterNum);
	
	for (uint32 i = 0; i < parameterNum; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC desc;
		D3D_ERROR_CHECK(m_Reflection->GetInputParameterDesc(i, &desc));

		// セマンティクスの文字列の参照を途切れさせないために一芝居
		Hash160 hash = CHash160(desc.SemanticName);
		D3D12_INPUT_ELEMENT_DESC elementDesc =
		{
			RHash160(hash),
			desc.SemanticIndex,
			GetDxgiFormat(desc.ComponentType, desc.Mask,desc.MinPrecision),
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		};
		
		input->push_back(elementDesc);
	}

	if (input->empty())
	{
		delete input;
		return false;
	}

	m_InputLayout.reset(input);

	ReleaseD3DPtr(m_Reflection);
	return true;
}

bool CPixelShader::Initialize(const FInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	FInitializerBase* notConstant = CCast<FInitializerBase*>(_Initializer);
	TargetSet(PCast<FInitializerMiddle*>(notConstant)) = CHash160("ps_5_0");
	return CShaderBase::Initialize(_Initializer);
}

bool CHullShader::Initialize(const FInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	FInitializerBase* notConstant = CCast<FInitializerBase*>(_Initializer);
	TargetSet(PCast<FInitializerMiddle*>(notConstant)) = CHash160("hs_5_0");
	return CShaderBase::Initialize(_Initializer);
}

bool CDomainShader::Initialize(const FInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	FInitializerBase* notConstant = CCast<FInitializerBase*>(_Initializer);
	TargetSet(PCast<FInitializerMiddle*>(notConstant)) = CHash160("ds_5_0");
	return CShaderBase::Initialize(_Initializer);
}

bool CGeometoryShader::Initialize(const FInitializerBase* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);
	FInitializerBase* notConstant = CCast<FInitializerBase*>(_Initializer);
	TargetSet(PCast<FInitializerMiddle*>(notConstant)) = CHash160("gs_5_0");
	return CShaderBase::Initialize(_Initializer);
}
