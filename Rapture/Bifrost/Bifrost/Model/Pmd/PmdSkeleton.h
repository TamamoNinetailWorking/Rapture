#pragma once

#include <vector>
#include <map>

#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FPmdSkeletonInitializer;

struct FPmdBoneNode;

class CVmdParser;

class CPmdSkeleton
{
private:

	using FBoneMatrices = std::vector<ATLANTIS_NAMESPACE::Glue::FMatrix>;
	using FBoneNodeTable = std::map<Hash160, FPmdBoneNode*>;

public:

	bool Initialize(const FPmdSkeletonInitializer* _Initializer);
	void Finalize();

	const FBoneMatrices* GetBoneMatrices() const;
	uint32 GetBoneMatricesNum() const;
	uint32 GetBoneMatricesSize() const;

	CPmdSkeleton();
	~CPmdSkeleton();

private:

	ObjectPtr(FBoneMatrices) m_Matrices = nullptr;
	ObjectPtr(FBoneNodeTable) m_Table = nullptr;

#define VMD_SKELETON_TEST
#ifdef VMD_SKELETON_TEST

	ObjectPtr(CVmdParser) m_VmdParser = nullptr;

#endif

#ifdef VMD_SKELETON_TEST
public:

	void PlayAnimation();
	void MotionUpdate(float _DeltaTime);
	bool IsPlay();

private:

	void MatrixReset();
	void MatrixUpdate();

#endif

private:

	bool CreateBoneNodeTable(const FPmdSkeletonInitializer* _Initializer);

};

BIFROST_NAMESPACE_END