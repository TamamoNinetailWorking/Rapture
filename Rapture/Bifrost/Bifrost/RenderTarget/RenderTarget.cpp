#include "RenderTarget.h"
#include "RenderTargetInitializer.h"

#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResource.h>
#include <Atlantis/DirectX12/RenderTargetView/RenderTargetViewResourceInitializer.h>

#include <eden/include/Resource/ResourceManager.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Rendering/RenderingSubsystem.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

#include <Atlantis/RHIProccessor/RHIProcessor.h>
#include <Atlantis/DirectX12/MainDevice/MainDevice.h>
#include <Atlantis/DirectX12/SwapChain/SwapChain.h>
#include <Atlantis/RHIProccessor/RHIRenderTargetView.h>
#include <Atlantis/DirectX12/DirectXHelper/d3dx12.h>

#include <Atlantis/RHIProccessor/RHIRenderTargetDefine.h>


USING_BIFROST;
USING_ATLANTIS;

bool CRenderTarget::Initialize(const FRenderTargetInitializer* _Initializer)
{
	do
	{
		CHECK_RESULT_BREAK(_Initializer);

		CHECK_RESULT_BREAK(CreateRenderTargetView(_Initializer));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CRenderTarget::Finalize()
{
	IResourceSubsystem::ManagerPtr TexManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();
	CHECK(TexManager);

	IResourceSubsystem::ManagerPtr RTVManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetRenderTargetViewResourceManagerEdit();
	CHECK(RTVManager);

	RTVManager->DeleteResource(m_RTVHandle);
	TexManager->DeleteResource(m_TextureHandle);
}

const ATLANTIS_NAMESPACE::CRenderTargetViewResource* CRenderTarget::GetRenderTargetView() const
{
	IResourceSubsystem::ManagerPtr RTVManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetRenderTargetViewResourceManagerEdit();
	if (RTVManager == nullptr) { return nullptr; };

	return PCast<const ATLANTIS_NAMESPACE::CRenderTargetViewResource*>(RTVManager->SearchResourceRef(m_RTVHandle));
}

bool CRenderTarget::CreateRenderTargetView(const FRenderTargetInitializer* _Initializer)
{
	CHECK_RESULT_FALSE(_Initializer);

	CRenderingSubsystem* Subsystem = CSubsystemServiceLocator::GetRenderingSubsystemEdit();
	CHECK_RESULT_FALSE(Subsystem);

	const auto& Processor = Subsystem->GetProcessor();
	CHECK_RESULT_FALSE(Processor);
	const auto& MainDevice = Processor->GetDevice();
	CHECK_RESULT_FALSE(MainDevice);
	const auto& Device = MainDevice->GetDevice();
	CHECK_RESULT_FALSE(Device);

	const auto& SystemRTV = Subsystem->GetRTV();
	CHECK_RESULT_FALSE(SystemRTV);
	const auto& SwapChain = SystemRTV->GetSwapChain();
	CHECK_RESULT_FALSE(SwapChain);
	const auto& BackBuffers = SwapChain->GetBackBuffer();
	CHECK_RESULT_FALSE(BackBuffers);
	const auto& BackBuffer1 = BackBuffers->at(0);
	CHECK_RESULT_FALSE(BackBuffer1);
	const auto& Desc = BackBuffer1->GetDesc();

	const D3D12_RESOURCE_DESC* pDesc = nullptr;
	if (_Initializer->pResDesc == nullptr)
	{
		pDesc = &Desc;
	}
	else
	{
		pDesc = PCast<const D3D12_RESOURCE_DESC*>(_Initializer->pResDesc);
	}

	const CTextureResource* TexRes = nullptr;

	{
		IResourceSubsystem::ManagerPtr TexManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetTextureResourceManagerEdit();
		CHECK_RESULT_FALSE(TexManager);

		FTextureInitializer initializer = {};
		initializer.Name = _Initializer->Name;
		initializer.Device = Device;
		initializer.HeapType = _Initializer->HeapType;
		initializer.pResDesc = pDesc;
		initializer.ClearValue = _Initializer->ClearValue;

		m_TextureHandle = TexManager->SearchCreateResource(&initializer);
		CHECK_RESULT_FALSE(TexManager->IsValidHandle(m_TextureHandle));

		TexRes = PCast<const CTextureResource*>(TexManager->SearchResourceRef(m_TextureHandle));
	}

	{
		IResourceSubsystem::ManagerPtr RTVManager = CSubsystemServiceLocator::GetResourceSubsystemEdit()->GetRenderTargetViewResourceManagerEdit();
		CHECK_RESULT_FALSE(RTVManager);

		FRenderTargetViewResourceInitializer initializer = {};
		initializer.Name = _Initializer->Name;
		initializer.Device = Device;
		initializer.ResPtr = TexRes->GetResource();
		initializer.Format = SCast<Glue::EDataFormat>(Desc.Format);
		initializer.RTVDimension = _Initializer->Dimension;

		m_RTVHandle = RTVManager->SearchCreateResource(&initializer);
		CHECK_RESULT_FALSE(RTVManager->IsValidHandle(m_RTVHandle));
	}

	return true;
}
