#pragma once

/*
*    �@>>Handle�͕ʂ̃w�b�_�[�ɒ�`���������ق���������������Ȃ��BHandle�����Ŏ��Ƃ��ɃI�[�o�[�w�b�h���傫���Ȃ�
      >>Resource�N���X��Initializer���������R�ŕʒ�`�����ق����ǂ�����
* 
*/

#include <list>

EDENS_NAMESPACE_BEGIN

class CResource;

struct FManagementResource
{
private:
	friend class CResourceManager;
	
	ObjectPtr(CResource) ResPtr = nullptr;
	sint32 RefCount = 0;
};


// �n���h���Œ���iterator�����͔̂�r�������ʓ|�ɂȂ�
// �L���E�������肪�ȒP�ɂ͍s���Ȃ�
// >> iterator����̏�ԂŔ�r���悤�Ƃ����exception�����ł���

typedef std::list<FManagementResource> FResourceList;

struct FResourceHandle
{
private:
	friend class CResourceManager;

	typedef FResourceList::iterator Handle;

	Handle iterator = {};

	bool isActive = false;

public:

	bool operator==(const FResourceHandle& _handle) const
	{
		return iterator == _handle.iterator;
	}

	bool operator !=(const FResourceHandle& _handle) const
	{
		return *this != _handle;
	}
};

static const FResourceHandle INVALID_RESOURCE_HANDLE = FResourceHandle();

EDENS_NAMESPACE_END