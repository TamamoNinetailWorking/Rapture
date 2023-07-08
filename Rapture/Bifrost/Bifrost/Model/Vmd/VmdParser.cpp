#include "VmdParser.h"
#include "VmdDefine.h"

#include <Atlantis/DirectX12/GlueMath.h>

#include <eden/include/utility/ender_utility.h>

#include <eden/include/gadget/serializer/serializer.h>

USING_BIFROST;
USING_ATLANTIS;

EDENS_NAMESPACE_USING;

using namespace std;

using namespace ATLANTIS_NAMESPACE::Glue;

class CVmdParser::Impl
{
public:

	struct FDeserializeMotionData
	{
	public:

		struct Forward
		{
			uint8 Name[VmdBoneNameLength] = {};
		};
		
		struct Backward
		{
			uint32 FrameNo = 0;
			Vector3 Locate = {};
			Vector4 Quat = {};
			uint8 Interpolation[VmdMotionInterpolateDataLength] = {};
		};

	public:
		Forward ForwardData = {};
	private:
		uint8 padding01 = {};
	public:
		Backward BackwardData = {};		
	};

	union FVmdMotionDataUnion
	{
		FVmdMotionData Data = {};
		FDeserializeMotionData desData;
	};

	struct FDeserializeBlendShapeData
	{
	public:

		struct Forward
		{
			uint8 Name[VmdBoneNameLength] = {};
		};

		struct Backward
		{
			uint32 FlameNo = 0;
			float Weight = 0.f;
		};

	public:
		Forward ForwardData = {};
	private:
		uint8 padding01 = {};
	public:
		Backward BackwardData = {};
	};

	union FVmdBlendShapeDataUnion
	{
		FVmdBlendShapeData Data = {};
		FDeserializeBlendShapeData desData;
	};

	struct FDeserializeCameraData
	{
	public:

		struct Data
		{
			uint32 FlameNo = 0;
			float Distance = 0.f;
			Vector3 Locate = {};
			Vector3 Rotate = {};
			uint8 Interpolation[VmdCameraInterpolateDataSize] = {};
			uint16 FieldOfView = 0;
			bool IsNotPerspective = true;
		};

	public:
		Data BackwardData = {};
	};

	union FVmdCameraDataUnion
	{
		FVmdCameraData Data = {};
		FDeserializeCameraData desData;
	};

	struct FDeserializeLightData
	{
	public:
		struct Data
		{
			uint32 FlameNo = 0;
			Vector3 Color = {};
			Vector3 Locate = {};
		};
	public:
		Data BackwardData = {};
	};

	union FVmdLightDataUnion
	{
		FVmdLightData Data = {};
		FDeserializeLightData desData;
	};

	struct FDeserializeSelfShadowData
	{
	public:
		struct Forward
		{
			uint32 FlameNo = 0;
			uint8 Mode = 0;
		};
	public:
		Forward ForwardData = {};
	private:
		uint8 padding01 = {};
	public:
		float Distance = 0.f;
	};

	union FVmdSelfShadowDataUnion
	{
		FVmdSelfShadowData Data = {};
		FDeserializeSelfShadowData desData;
	};

	typedef std::vector<FVmdMotionDataUnion> FVmdMotions;
	typedef std::vector<FVmdBlendShapeDataUnion> FVmdBlendShapes;
	typedef std::vector<FVmdCameraDataUnion> FVmdCameras;
	typedef std::vector<FVmdLightDataUnion> FVmdLights;
	typedef std::vector<FVmdSelfShadowDataUnion> FVmdSelfShadows;

public:

	bool DeserializeHeader();
	bool DeserializeMotionData();
	bool DeserializeBlendShapeData();
	bool DeserializeCameraData();
	bool DeserializeLightData();
	bool DeserializeSelfShadowData();

private:

	friend class CVmdParser;

	ObjectPtr(EDENS_NAMESPACE::CSerializer) m_Serializer = nullptr;

	ObjectPtr(FVmdMotions) m_Motions = nullptr;
	ObjectPtr(FVmdBlendShapes) m_Shapes = nullptr;
	ObjectPtr(FVmdCameras) m_Cameras = nullptr;
	ObjectPtr(FVmdLights) m_Lights = nullptr;
	ObjectPtr(FVmdSelfShadows) m_SelfShadows = nullptr;

};

CVmdParser::CVmdParser()
{
	m_Impl = new Impl();
}

CVmdParser::~CVmdParser()
{
	EDENS_NAMESPACE::Delete(m_Impl);
}

bool CVmdParser::ParseData(const uint8* _Data, uint32 _Size)
{
	if (_Data == nullptr) { return false; };
	if (_Size <= 0) { return false; };

	m_Impl->m_Serializer = new CSerializer();
	if (m_Impl->m_Serializer == nullptr) { return false; };

	m_Impl->m_Serializer->Initialize(_Data, _Size);

	do 
	{
		// モーションデータ以外はデータが無くてもデシリアライズできるように許容
		CHECK_RESULT_BREAK(m_Impl->DeserializeHeader());
		CHECK_RESULT_BREAK(m_Impl->DeserializeMotionData());
		CHECK_RESULT_BREAK(m_Impl->DeserializeBlendShapeData());
		CHECK_RESULT_BREAK(m_Impl->DeserializeCameraData());
		CHECK_RESULT_BREAK(m_Impl->DeserializeLightData());
		CHECK_RESULT_BREAK(m_Impl->DeserializeSelfShadowData());
		
		m_Impl->m_Serializer->Finalize();

		return true;
	} while (0);


	Reset();
	return false;
}

void CVmdParser::Reset()
{
	CHECK(m_Impl);

	EDENS_NAMESPACE::Delete(m_Impl->m_SelfShadows);
	EDENS_NAMESPACE::Delete(m_Impl->m_Lights);
	EDENS_NAMESPACE::Delete(m_Impl->m_Cameras);
	EDENS_NAMESPACE::Delete(m_Impl->m_Shapes);
	EDENS_NAMESPACE::Delete(m_Impl->m_Motions);

	EDENS_NAMESPACE::FinalizeObject(m_Impl->m_Serializer);
}

const FVmdMotionData* CVmdParser::GetMotionData() const
{
	if (!m_Impl) { return nullptr; }
	if (!m_Impl->m_Motions) { return nullptr; }
	return RCast<const FVmdMotionData*>(m_Impl->m_Motions->data());
}

uint32 CVmdParser::GetMotionDataNum() const
{
	if (!m_Impl) { return 0; }
	if (!m_Impl->m_Motions) { return 0; }
	return SCast<uint32>(m_Impl->m_Motions->size());
}

const FVmdBlendShapeData* CVmdParser::GetFaceData() const
{
	if (!m_Impl) { return nullptr; }
	if (!m_Impl->m_Shapes) { return nullptr; }
	return RCast<const FVmdBlendShapeData*>(m_Impl->m_Shapes->data());
}

uint32 CVmdParser::GetFaceDataNum() const
{
	if (!m_Impl) { return 0; }
	if (!m_Impl->m_Shapes) { return 0; }
	return SCast<uint32>(m_Impl->m_Shapes->size());
}

const FVmdCameraData* CVmdParser::GetCameraData() const
{
	if (!m_Impl) { return nullptr; }
	if (!m_Impl->m_Cameras) { return nullptr; }
	return RCast<const FVmdCameraData*>(m_Impl->m_Cameras->data());
}

uint32 CVmdParser::GetCameraDataNum() const
{
	if (!m_Impl) { return 0; }
	if (!m_Impl->m_Cameras) { return 0; }
	return SCast<uint32>(m_Impl->m_Cameras->size());
}

const FVmdLightData* CVmdParser::GetLightData() const
{
	if (!m_Impl) { return nullptr; }
	if (!m_Impl->m_Lights) { return nullptr; }
	return RCast<const FVmdLightData*>(m_Impl->m_Lights->data());
}

uint32 CVmdParser::GetLightDataNum() const
{
	if (!m_Impl) { return 0; }
	if (!m_Impl->m_Lights) { return 0; }
	return SCast<uint32>(m_Impl->m_Lights->size());
}

const FVmdSelfShadowData* CVmdParser::GetSelfShadowData() const
{
	if (!m_Impl) { return nullptr; }
	if (!m_Impl->m_SelfShadows) { return nullptr; }
	return RCast<const FVmdSelfShadowData*>(m_Impl->m_SelfShadows->data());
}

uint32 CVmdParser::GetSelfShadowDataNum() const
{
	if (!m_Impl) { return 0; }
	if (!m_Impl->m_SelfShadows) { return 0; }
	return SCast<uint32>(m_Impl->m_SelfShadows->size());
}

bool CVmdParser::Impl::DeserializeHeader()
{
	CHECK_RESULT_FALSE(m_Serializer);

	constexpr uint8 HeaderSize = 50;

	CHECK_RESULT_FALSE(m_Serializer->IncrementReadPosition(HeaderSize));

	return true;
}

bool CVmdParser::Impl::DeserializeMotionData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Motions = new FVmdMotions();
	CHECK_RESULT_FALSE(m_Motions);

	uint32 motionNum = 0;
	CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&motionNum, sizeof(motionNum)));

	m_Motions->resize(motionNum);

	for (auto& motion : *m_Motions)
	{
		auto& dest = motion.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.ForwardData, sizeof(FDeserializeMotionData::Forward)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeMotionData::Backward)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeBlendShapeData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Shapes = new FVmdBlendShapes();
	CHECK_RESULT_FALSE(m_Shapes);

	uint32 shapeNum = 0;
	//CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&shapeNum, sizeof(shapeNum)));
	if (!m_Serializer->ReadDataBlob(&shapeNum, sizeof(shapeNum)))
	{
		return true;
	}

	m_Shapes->resize(shapeNum);

	for (auto& shape : *m_Shapes)
	{
		FDeserializeBlendShapeData& dest = shape.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.ForwardData, sizeof(FDeserializeBlendShapeData::Forward)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeBlendShapeData::Backward)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeCameraData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Cameras = new FVmdCameras();
	CHECK_RESULT_FALSE(m_Cameras);

	uint32 cameraNum = 0;
	//CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&cameraNum, sizeof(cameraNum)));
	if (!m_Serializer->ReadDataBlob(&cameraNum, sizeof(cameraNum)))
	{
		return true;
	}

	m_Cameras->resize(cameraNum);

	for (auto& camera : *m_Cameras)
	{
		auto& dest = camera.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeCameraData::Data)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeLightData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Lights = new FVmdLights();
	CHECK_RESULT_FALSE(m_Lights);

	uint32 lightNum = 0;
	//CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&lightNum, sizeof(lightNum)));
	if (!m_Serializer->ReadDataBlob(&lightNum, sizeof(lightNum)))
	{
		return true;
	}

	m_Lights->resize(lightNum);

	for (auto& light : *m_Lights)
	{
		auto& dest = light.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeLightData::Data)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeSelfShadowData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_SelfShadows = new FVmdSelfShadows();

	uint32 shadowNum = 0;
	//CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&shadowNum, sizeof(shadowNum)));
	
	if (!m_Serializer->ReadDataBlob(&shadowNum, sizeof(shadowNum)))
	{
		return true;
	}

	m_SelfShadows->resize(shadowNum);

	for (auto& shadow : *m_SelfShadows)
	{
		auto& dest = shadow.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.ForwardData, sizeof(FDeserializeSelfShadowData::Forward)));
		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.Distance, sizeof(FDeserializeSelfShadowData::Distance)));
	}

	return true;
}
