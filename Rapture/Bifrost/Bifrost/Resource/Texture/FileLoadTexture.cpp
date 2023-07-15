#include "FileLoadTexture.h"

#include <eden/include/utility/ender_utility.h>

#include <eden/include/gadget/FileLoader/FileLoader.h>
#include <Atlantis/DirectX12/Texture/Texture.h>
#include <Atlantis/DirectX12/Texture/TextureResourceInitializer.h>
#include <Atlantis/DirectX12/Texture/TextureResource.h>
#include <Atlantis/Resource/TextureResourceManager.h>

#include <Atlantis/DirectX12/DirectXTex/DirectXTex.h>

#include <Bifrost/Subsystem/ServiceLocator/SubsystemServiceLocator.h>
#include <Bifrost/Subsystem/Resource/ResourceSubsystemInterface.h>

USING_BIFROST;
EDENS_NAMESPACE_USING;
USING_ATLANTIS;

class CFileLoadTexture::Impl 
{
public:

	bool LoadFile(CFileLoader& _Loader,const CFileLoadTexture::FInitializer& _Initializer);
	bool CreateTexture(CTexture& _Texture,const CFileLoader& _Loader);
	const FResourceHandle CreateTextureResource(const CTexture& _Texture, const CFileLoadTexture::FInitializer& _Initializer);

};

bool CFileLoadTexture::Initialize(const FInitializer& _Initializer)
{
	do
	{
		CFileLoader loader = {};
		CHECK_RESULT_BREAK(m_Impl->LoadFile(loader, _Initializer));
		
		CTexture texture = {};
		CHECK_RESULT_BREAK(m_Impl->CreateTexture(texture, loader));

		m_Handle = m_Impl->CreateTextureResource(texture,_Initializer);

		CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();
		CHECK_RESULT_BREAK(manager->IsValidHandle(m_Handle));

		return true;
	} while (0);

	Finalize();
	return false;
}

void CFileLoadTexture::Finalize()
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();
	manager->DeleteResource(m_Handle);
	m_Handle = manager->GetInvalidHandle();
}

const FResourceHandle& CFileLoadTexture::GetResourceHandle() const
{
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();
	manager->AddRef(m_Handle);
	return m_Handle;
}

CFileLoadTexture::CFileLoadTexture()
{
	m_Impl = new Impl();
}

CFileLoadTexture::~CFileLoadTexture()
{
	Delete(m_Impl);
}

bool CFileLoadTexture::Impl::LoadFile(CFileLoader& _Loader, const CFileLoadTexture::FInitializer& _Initializer)
{
	return _Loader.FileLoad(_Initializer.FileName);
}

bool CFileLoadTexture::Impl::CreateTexture(CTexture& _Texture, const CFileLoader& _Loader)
{
	CTexture::FInitializer initializer = {};
	initializer.FileNameHash = _Loader.GetFileName();
	initializer.Data = _Loader.GetData();
	initializer.DataSize = _Loader.GetSize();

	return _Texture.Initialize(initializer);
}

const FResourceHandle CFileLoadTexture::Impl::CreateTextureResource(const CTexture& _Texture, const CFileLoadTexture::FInitializer& _Initializer)
{
	FTextureInitializer initializer = {};
	initializer.Device = _Initializer.Device;
	initializer.Name = _Initializer.FileName;
	initializer.ResDesc.Width = SCast<uint32>(_Texture.GetMetaData()->width);
	initializer.ResDesc.Height = SCast<uint32>(_Texture.GetMetaData()->height);
	initializer.ResDesc.DepthOrArraySize = SCast<uint32>(_Texture.GetMetaData()->arraySize);
	initializer.ResDesc.MipLevels = SCast<uint32>(_Texture.GetMetaData()->mipLevels);
	initializer.ResDesc.Format = SCast<Glue::EDataFormat>(_Texture.GetMetaData()->format);
	initializer.ResDesc.Dimension = SCast<Glue::EResourceDimension>(_Texture.GetMetaData()->dimension);
	initializer.SourceData = _Texture.GetImage()->pixels;
	initializer.RowPitch = SCast<uint32>(_Texture.GetImage()->rowPitch);
	initializer.SlicePitch = SCast<uint32>(_Texture.GetImage()->slicePitch);
	
	CResourceManager* manager = CSubsystemServiceLocator::GetResourceSubsystem()->GetTextureResourceManager();

	return manager->SearchCreateResource(&initializer);
}
