#include <d3d12.h>

#include <Atlantis/DirectX12/MainDevice/MainDevice.h>

#include <Bifrost/Resource/PSO/PipelineStateObject.h>
#include <Bifrost/Resource/PSO/PipelineStateObjectInitializer.h>

#include <Atlantis/DirectX12/Shader/GraphicsShaderInitializer.h>
#include <eden/include/gadget/FileLoader/FileLoader.h>

#include <string>
#include <eden/include/utility/StringUtility.h>


EDENS_NAMESPACE_USING;
USING_ATLANTIS;
USING_BIFROST;

//using namespace Glue;

namespace Test
{

	CPipelineStateObject* m_Pso = nullptr;

	using String = std::string;

	void GraphicsPipelineTest(CDX12MainDevice* _Device)
	{
		{
			m_Pso = new CPipelineStateObject();
		}

		{
			FPipelineStateObjectInitializer initializer = {};

			initializer.Device = _Device;
			initializer.Flag = Glue::ERootSignatureFlag::ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			D3D12_DESCRIPTOR_RANGE descTableRange[3] = {};
			D3D12_ROOT_PARAMETER rootParam[2] = {};
			D3D12_STATIC_SAMPLER_DESC samplerDesc[2] = {};
			{
				// ディスクリプタ―テーブル << シェーダーで扱うレジスタの設定

				descTableRange[0].NumDescriptors = 1;
				descTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				descTableRange[0].BaseShaderRegister = 0;
				descTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				descTableRange[1].NumDescriptors = 1;
				descTableRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
				descTableRange[1].BaseShaderRegister = 1;
				descTableRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

				// テクスチャ用テーブル
				descTableRange[2].NumDescriptors = 4; //(普通のテクスチャとsphとspaとtoon)
				descTableRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
				descTableRange[2].BaseShaderRegister = 0;
				descTableRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


				rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
				rootParam[0].DescriptorTable.pDescriptorRanges = &descTableRange[0];
				rootParam[0].DescriptorTable.NumDescriptorRanges = 1;

				rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
				rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				rootParam[1].DescriptorTable.pDescriptorRanges = &descTableRange[1];
				rootParam[1].DescriptorTable.NumDescriptorRanges = 2;


				samplerDesc[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
				samplerDesc[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc[0].MaxLOD = D3D12_FLOAT32_MAX;
				samplerDesc[0].MinLOD = 0.0f;
				samplerDesc[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
				samplerDesc[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
				samplerDesc[0].ShaderRegister = 0;

				samplerDesc[1] = samplerDesc[0];
				samplerDesc[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc[1].ShaderRegister = 1;


			}

			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
			{


				desc.BlendState.AlphaToCoverageEnable = true;
				desc.BlendState.IndependentBlendEnable = false;// こいつがfalseならRenderTargetの設定は最初の1つだけでOKになる Independent => 個別に


				D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
				renderTargetBlendDesc.BlendEnable = false;
				renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				renderTargetBlendDesc.LogicOpEnable = false;

				desc.BlendState.RenderTarget[0] = renderTargetBlendDesc;


				desc.RasterizerState.MultisampleEnable = false;
				desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
				desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
				//desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
				desc.RasterizerState.DepthClipEnable = true;

				desc.RasterizerState.FrontCounterClockwise = false;
				desc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
				desc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
				desc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
				desc.RasterizerState.AntialiasedLineEnable = false;
				desc.RasterizerState.ForcedSampleCount = 0;
				desc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


				desc.DepthStencilState.DepthEnable = true;
				desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
				desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; // 小さい方（近い方）を採用
				desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
				desc.DepthStencilState.StencilEnable = false;

				desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
				desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

				desc.SampleDesc.Count = 1;
				desc.SampleDesc.Quality = 0;

				desc.NumRenderTargets = 1;
				desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

				
				initializer.pPipelineDesc = &desc;
			}

			CFileLoader vsLoader = {};
			CFileLoader psLoader = {};
			FVertexShaderInitializer vsInit = {};
			FPixelShaderInitializer psInit = {};
			{
				{
					vsLoader.FileLoad("resource/shader/PmdVertexShader.hlsl");
					vsInit.Data = vsLoader.GetData();
					vsInit.Size = vsLoader.GetSize();

					String fileAndFuncName = StringUtility::CombineText("resource/shader/PmdVertexShader.hlsl", "main", '*');
					vsInit.Name = CHash160(fileAndFuncName);
				}

				vsInit.Device = _Device->GetDevice();
				vsInit.Type = EShaderType::SHADER_TYPE_VERTEX;

				{
					psLoader.FileLoad("resource/shader/PmdPixelShader.hlsl");
					psInit.Data = psLoader.GetData();
					psInit.Size = psLoader.GetSize();

					String fileAndFuncName = StringUtility::CombineText("resource/shader/PmdPixelShader.hlsl", "main", '*');
					psInit.Name = CHash160(fileAndFuncName);
				}

				psInit.Type = EShaderType::SHADER_TYPE_PIXEL;
			}

			{
				String name = RHash160(vsInit.Name);
				name += RHash160(psInit.Name);

				initializer.Name = CHash160(name);
			}

			initializer.pParameters = rootParam;
			initializer.NumParameters = 2;
			initializer.pStaticSamplers = samplerDesc;
			initializer.NumStaticSamplers = 2;
			initializer.VertexShaderInit = &vsInit;
			initializer.PixelShaderInit = &psInit;

			m_Pso->Initialize(&initializer);

			vsLoader.ResetData();
			psLoader.ResetData();

			//pso.Finalize();
		}
	}
};