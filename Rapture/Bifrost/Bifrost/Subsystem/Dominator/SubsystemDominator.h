#pragma once

#include <eden/include/template/NonCopyable.h>

//ATLANTIS_NAMESPACE_BEGIN
BIFROST_NAMESPACE_BEGIN

class IResourceSubsystem;
class CUpdateProcessorSubsystem;
class CRenderingSubsystem;
class CActorSubsystem;
class CCameraSubsystem;
class CLightSubsystem;
class CUpdateIntervalSubsystem;

// たとえば
/*
#define REGIST_DOMINATOR_SUBSYSTEM(_Subsystem) \
public:	\
	void Set##_Subsystem(C##_Subsystem* _Subsystem); \
private: \
	C##_Subsystem* m_##_Subsystem = nullptr; \
*/

// こんな変数から関数までの一括定義マクロを作ってしまって、
// SubsystemDominator自体を自動作成できないだろうか

// 名前空間が違うSubsystemを管理する際に通用しなくなるかもしれないが・・・
// ソースコードを作成するためのスクリプトは一応イメージできる(Python等で)
// それを管理・変更するためのツールのベースはExcelのような列挙しやすいスプレッドシートかな

// とりあえず今はマクロで多少は楽にしたいところ

#define REGIST_DOMINATOR_PROCEDURE(_HeadChar,_Class) \
public: \
	void Set##_Class(_HeadChar##_Class##* _##_Class) { m_##_Class = _##_Class; }; \
	const _HeadChar##_Class##* Get##_Class() const { return m_##_Class; }; \
	_HeadChar##_Class##* Get##_Class##Edit() const { return m_##_Class; }; \
private: \
	ObjectPtr(_HeadChar##_Class) m_##_Class = nullptr; \

#define REGIST_DOMINATOR_CLASS(_Class) \
	REGIST_DOMINATOR_PROCEDURE(C,_Class) \

#define REGIST_DOMINATOR_INTERFACE(_Interface) \
	REGIST_DOMINATOR_PROCEDURE(I,_Interface) \

class CSubsystemDominator : public CNonCopyable
{
public:

	bool Initialize();
	void Finalize();

	CSubsystemDominator() {};
	~CSubsystemDominator() {};

public:

	REGIST_DOMINATOR_INTERFACE(ResourceSubsystem);
	REGIST_DOMINATOR_CLASS(UpdateProcessorSubsystem);
	REGIST_DOMINATOR_CLASS(RenderingSubsystem);
	REGIST_DOMINATOR_CLASS(ActorSubsystem);
	REGIST_DOMINATOR_CLASS(CameraSubsystem);
	REGIST_DOMINATOR_CLASS(LightSubsystem);
	REGIST_DOMINATOR_CLASS(UpdateIntervalSubsystem);

#if 0

public:

	void SetResourceSubsystem(IResourceSubsystem* _ResSubsystem);
	void SetUpdaterSubsystem(CUpdateProcessorSubsystem* _UpdaterSubsystem);
	void SetRenderingSubsystem(CRenderingSubsystem* _RenderingSubsystem);
	void SetActorSubsystem(CActorSubsystem* _ActorSubsystem);
	void SetCameraSubsystem(CCameraSubsystem* _CameraSubsystem);
	void SetLightSubsystem(CLightSubsystem* _LightSubsystem);

	const IResourceSubsystem* GetResourceSubsystem() const { return m_ResourceSubsystem; };
	IResourceSubsystem* GetResourceSubsystemEdit() const { return m_ResourceSubsystem; };

	const CUpdateProcessorSubsystem* GetUpdaterSubsystem() const { return m_UpdaterSubsystem; };
	CUpdateProcessorSubsystem* GetUpdaterSubsystemEdit() const { return m_UpdaterSubsystem; };

	const CRenderingSubsystem* GetRenderingSubsystem() const { return m_RenderingSubsystem; };
	CRenderingSubsystem* GetRenderingSubsystemEdit() const { return m_RenderingSubsystem; };

	const CActorSubsystem* GetActorSubsystem() const { return m_ActorSubsystem; }
	CActorSubsystem* GetActorSubsystemEdit() const { return m_ActorSubsystem; }

	const CCameraSubsystem* GetCameraSubsystem() const { return m_CameraSubsystem; }
	CCameraSubsystem* GetCameraSubsystemEdit() const { return m_CameraSubsystem; }

	const CLightSubsystem* GetLightSubsystem() const { return m_LightSubsystem; }
	CLightSubsystem* GetLightSubsystemEdit() const { return chrome://vivaldi-webui/startpage?section=Speed-dials&background-color=#2e2f37m_LightSubsystem; }



private:

	ObjectPtr(IResourceSubsystem) m_ResourceSubsystem = nullptr;
	ObjectPtr(CUpdateProcessorSubsystem) m_UpdaterSubsystem = nullptr;
	ObjectPtr(CRenderingSubsystem) m_RenderingSubsystem = nullptr;
	ObjectPtr(CActorSubsystem) m_ActorSubsystem = nullptr;
	ObjectPtr(CCameraSubsystem) m_CameraSubsystem = nullptr;
	ObjectPtr(CLightSubsystem) m_LightSubsystem = nullptr;

#endif

};

#undef REGIST_DOMINATOR_INTERFACE
#undef REGIST_DOMINATOR_CLASS
#undef REGIST_DOMINATOR_PROCEDURE

BIFROST_NAMESPACE_END
//ATLANTIS_NAMESPACE_END