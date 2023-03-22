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
			Vector3 Locate = {};
			Vector4 Quat = {};
			uint8 Interpolation[VmdMotionInterpolateDataLength] = {};
		};

	public:
		Forward ForwardData = {};
	private:
		uint8 padding01 = {};
	public:
		uint16 FrameNo = 0;
	private:
		uint16 padding02 = {};
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

	public:
		Forward ForwardData = {};
	private:
		uint8 padding01 = {};
	public:
		uint16 FlameNo = 0;
	private:
		uint16 padding02 = {};
	public:
		float Weight = 0.f;
	};

	union FVmdBlendShapeDataUnion
	{
		FVmdBlendShapeData Data = {};
		FDeserializeBlendShapeData desData;
	};

	struct FDeserializeCameraData
	{
	public:

		struct Backward
		{
			float Distance = 0.f;
			Vector3 Locate = {};
			Vector3 Rotate = {};
			uint8 Interpolation[VmdCameraInterpolateDataSize] = {};
			uint16 FieldOfView = 0;
			bool IsNotPerspective = true;
		};

	public:
		uint16 FlameNo = 0;
	private:
		uint16 padding01 = {};
	public:
		Backward BackwardData = {};
	};

	union FVmdCameraDataUnion
	{
		FVmdCameraData Data = {};
		FDeserializeCameraData desData;
	};

	struct FDeserializeLightData
	{
	public:
		struct Backward
		{
			Vector3 Color = {};
			Vector3 Locate = {};
		};
	public:
		uint16 FlameNo = 0;
	private:
		uint16 padding01 = {};
	public:
		Backward BackwardData = {};
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
			uint16 FlameNo = 0;
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

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.FrameNo, sizeof(FDeserializeMotionData::FrameNo)));

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
	CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&shapeNum, sizeof(shapeNum)));

	m_Shapes->resize(shapeNum);

	for (auto& shape : *m_Shapes)
	{
		FDeserializeBlendShapeData& dest = shape.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.ForwardData, sizeof(FDeserializeBlendShapeData::Forward)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.FlameNo, sizeof(FDeserializeBlendShapeData::FlameNo)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.Weight, sizeof(FDeserializeBlendShapeData::Weight)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeCameraData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Cameras = new FVmdCameras();
	CHECK_RESULT_FALSE(m_Cameras);

	uint32 cameraNum = 0;
	CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&cameraNum, sizeof(cameraNum)));

	m_Cameras->resize(cameraNum);

	for (auto& camera : *m_Cameras)
	{
		auto& dest = camera.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.FlameNo, sizeof(FDeserializeCameraData::FlameNo)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeCameraData::Backward)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeLightData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_Lights = new FVmdLights();
	CHECK_RESULT_FALSE(m_Lights);

	uint32 lightNum = 0;
	CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&lightNum, sizeof(lightNum)));

	m_Lights->resize(lightNum);

	for (auto& light : *m_Lights)
	{
		auto& dest = light.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.FlameNo, sizeof(FDeserializeLightData::FlameNo)));

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.BackwardData, sizeof(FDeserializeLightData::Backward)));
	}

	return true;
}

bool CVmdParser::Impl::DeserializeSelfShadowData()
{
	CHECK_RESULT_FALSE(m_Serializer);

	m_SelfShadows = new FVmdSelfShadows();

	uint32 shadowNum = 0;
	CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&shadowNum, sizeof(shadowNum)));

	m_SelfShadows->resize(shadowNum);

	for (auto& shadow : *m_SelfShadows)
	{
		auto& dest = shadow.desData;

		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.ForwardData, sizeof(FDeserializeSelfShadowData::Forward)));
		CHECK_RESULT_FALSE(m_Serializer->ReadDataBlob(&dest.Distance, sizeof(FDeserializeSelfShadowData::Distance)));
	}

	return true;
}
