#pragma once

/*

���[�v�̃C���[�W��


Physics Update
Input Update
Actor Update

Rendering


�܂���Unity��Lifecycle�������낤��



Component��Update���̂��ƂȂǂ��l����ƁA
UnrealEngine��TickGroup���䂪�����̂�������Ȃ�

TickGroup�AVisibility��RendererComponent�̗L����
�X�V�������ɏ����ƕ`�惊�X�g�����

ActorManager->Update
	GroupSeparation
		PrePhysicsUpdate
		PhysicsUpdate
		PostPhysicsUpdate
		RenderUpdate

Component�̂��Ƃ��l����ƁAUpdateSubsystem�݂����Ȃ��̂��K�v��������Ȃ�
	>> Component��Manager�ŊǗ�����ƁAActor�Ɠ�d��ComponentList����������


�`�揈���͐؂蕪���Ă����΁A�����_�����O�X���b�h�Ƃ���Draw�R�[�����s��������؂蕪���₷����������Ȃ�
GPU�Ƀf�[�^�]�����Ă��Ȃ����̂ɂ��Ă̓����_�����O�X���b�h���ɃR�s�[����K�v���������肷�邾�낤��

*/

#include <list>

BIFROST_NAMESPACE_BEGIN

class CActor;

struct FActorInitializerBase;

class CActorManager
{
public:

	bool Initialize();
	void Finalize();

	
	//void Update(float _DeltaTime);

	template <class Actor>
	CActor* CreateActor(const FActorInitializerBase* _Initializer);

	void DeleteActor(const Hash160& _Name);
	void DeleteActor(const CActor* _Actor);

	void DestroyActors();

	CActorManager();
	~CActorManager();

private:
	
	using ActorList = std::list<CActor*>;

	ObjectPtr(ActorList) m_ActorList = nullptr;


	void ListFinalize();

};

BIFROST_NAMESPACE_END

#include "ActorManager.inl"