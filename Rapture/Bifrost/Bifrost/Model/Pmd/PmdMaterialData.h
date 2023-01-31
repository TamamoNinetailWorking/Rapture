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


#include <Atlantis/Material/MaterialInterface.h>

ATLANTIS_NAMESPACE_BEGIN

struct FMaterialInterfaceInitializerBase;
class CGraphicsPipeline;
class CRootSignature;

struct FMaterialGeometryBufferBase;

ATLANTIS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

class CPmdMaterialData : public ATLANTIS_NAMESPACE::IMaterialInterface
{
public:

	bool Initialize(const ATLANTIS_NAMESPACE::FMaterialInterfaceInitializerBase* _Initializer) override;
	void Finalize() override;

    void SetGeometryBuffer(ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* _Buffer) override;

	uint32 GetDrawIndex(uint32 _Index) const override;
	uint32 GetMaterialNum() const override;

    ATLANTIS_NAMESPACE::CGraphicsPipeline* GetGraphicsPipeline() const override;
    ATLANTIS_NAMESPACE::CRootSignature* GetRootSignature() const override;

    const ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBuffer() const override;
    ATLANTIS_NAMESPACE::FMaterialGeometryBufferBase* GetGeometryBufferEdit() const override;

	CPmdMaterialData();
	~CPmdMaterialData();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;
};

BIFROST_NAMESPACE_END