#pragma once

#include "VmdDefine.h"

BIFROST_NAMESPACE_BEGIN

class CVmdParser
{
public:

	bool ParseData(const uint8* _Data, uint32 _Size);

	void Reset();

	const FVmdMotionData* GetMotionData() const;
	uint32 GetMotionDataNum() const;
	const FVmdBlendShapeData* GetFaceData() const;
	uint32 GetFaceDataNum() const;
	const FVmdCameraData* GetCameraData() const;
	uint32 GetCameraDataNum() const;
	const FVmdLightData* GetLightData() const;
	uint32 GetLightDataNum() const;
	const FVmdSelfShadowData* GetSelfShadowData() const;
	uint32 GetSelfShadowDataNum() const;

	CVmdParser();
	~CVmdParser();

private:

	class Impl;
	ObjectPtr(Impl) m_Impl = nullptr;

};

BIFROST_NAMESPACE_END