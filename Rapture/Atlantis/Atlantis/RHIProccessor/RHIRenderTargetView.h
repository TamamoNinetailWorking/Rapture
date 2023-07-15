#pragma once

#include "RHIRenderTargetDefine.h"

ATLANTIS_NAMESPACE_BEGIN

class CSwapChain;
class CRenderTargetView;
class CDepthStencilView;

struct FRHIRenderTargetInitializer;

class CRHIRenderTargetView
{
public:

	bool Initialize(const FRHIRenderTargetInitializer* _Initializer);
	void Finalize();

	CSwapChain* GetSwapChain() const { return m_SwapChain; }
	CRenderTargetView* GetRenderTargetView(uint32 _Index) const { return m_RTVBuffer.at(_Index); };
	CDepthStencilView* GetDepthStencilView() const { return m_DepthStencilView; }
	CRenderTargetView* GetCurrentRenderTargetView() const;

	CRHIRenderTargetView();
	~CRHIRenderTargetView();

private:

	ObjectPtr(CSwapChain) m_SwapChain = nullptr;

	using RenderTargetArray = std::array<ObjectPtr(CRenderTargetView), RHI_RenderTargetViewNum>;
	RenderTargetArray m_RTVBuffer = {};

	ObjectPtr(CDepthStencilView) m_DepthStencilView = nullptr;

private:

	bool CreateSwapChain(const FRHIRenderTargetInitializer* _Initializer);
	bool CreateRenderTargetView(const FRHIRenderTargetInitializer* _Initializer);
	bool CreateDepthStencilView(const FRHIRenderTargetInitializer* _Initializer);
	
};

ATLANTIS_NAMESPACE_END