#pragma once

#include <eden/include/Resource/ResourceManagerPreDefine.h>

struct ID3D12Device;

EDENS_NAMESPACE_BEGIN

class CResource;

EDENS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

class CFileLoadTexture
{
public:

	struct FInitializer
	{
		ID3D12Device* Device = nullptr;
		Hash160 FileName = {};
	};

	bool Initialize(const FInitializer& _Initializer);
	void Finalize();

	const EDENS_NAMESPACE::FResourceHandle& GetResourceHandle() const;

	CFileLoadTexture();
	~CFileLoadTexture();

private:

	EDENS_NAMESPACE::FResourceHandle m_Handle = {};

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};


BIFROST_NAMESPACE_END