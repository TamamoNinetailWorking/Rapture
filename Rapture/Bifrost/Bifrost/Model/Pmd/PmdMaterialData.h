#pragma once

/*
*     PmdMaterialDataを作成する
     >>マテリアルデータをシリアライザーから吸い上げてHLSLに送り出すデータをまとめて保持する
       >>Pmdモデルを描画する際には必須
     >>TextureDataを持っている
        >>フライウェイトパターン的に、一度作成したリソースを再作成したくない
            >>特にtoonXX.bmp周り
        >> つまりこのタイミングでTextureのID3D12Resourceをパッキングしたクラスと、
            Textureの管理を行うSubsystemが必要になる
            >> Subsystemの起動時に白テクスチャや黒テクスチャを作るようにしておく
                >> 作るためのクラスオブジェクトはAtlantis側で定義しておくといいだろう
*/

#include <eden/include/Resource/Resource.h>

struct ID3D12DescriptorHeap;


EDENS_NAMESPACE_BEGIN

struct FResourceInitializerBase;

EDENS_NAMESPACE_END

//#include "PmdPreDefine.h"

BIFROST_NAMESPACE_BEGIN

class CPmdMaterialData : public EDENS_NAMESPACE::CResource
{
public:

	bool Initialize(const  EDENS_NAMESPACE::FResourceInitializerBase* _Initializer) override;
	void Finalize() override;

	ID3D12DescriptorHeap* GetDescriptorHeap() const;
	ID3D12DescriptorHeap* const* GetDescriptorHeapPtr() const;
	uint32 GetHeapStride() const;
	uint32 GetDrawIndex(uint32 _Index) const;
	uint32 GetMaterialNum() const;

	CPmdMaterialData();
	~CPmdMaterialData();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;
};

BIFROST_NAMESPACE_END