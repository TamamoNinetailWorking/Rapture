#pragma once

#include <vector>
#include <map>

#include <Atlantis/DirectX12/GlueMath.h>

BIFROST_NAMESPACE_BEGIN

struct FPmdSkeletonInitializer;

struct FPmdBoneNode;

class CPmdSkeleton
{
public:

	bool Initialize(const FPmdSkeletonInitializer* _Initializer);
	void Finalize();

	CPmdSkeleton();
	~CPmdSkeleton();

private:

	using FBoneMatrices = std::vector<ATLANTIS_NAMESPACE::Glue::FMatrix>;
	using FBoneNodeTable = std::map<Hash160, FPmdBoneNode*>;

	ObjectPtr(FBoneMatrices) m_Matrices = nullptr;
	ObjectPtr(FBoneNodeTable) m_Table = nullptr;

private:

	bool CreateBoneNodeTable(const FPmdSkeletonInitializer* _Initializer);

};

BIFROST_NAMESPACE_END