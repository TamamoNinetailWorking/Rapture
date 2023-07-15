#pragma once

#include <Bifrost/Subsystem/Updater/UpdateObjectType.h>

EDENS_NAMESPACE_BEGIN

class CObject;

EDENS_NAMESPACE_END

BIFROST_NAMESPACE_BEGIN

//struct FOnceExecuteParamBase {};

typedef std::function<void()> FOnceExecuteFunction;

struct FOnceExecuteData
{
	ObjectPtr(EDENS_NAMESPACE::CObject) ObjectPtr = nullptr;
	FOnceExecuteFunction Function = {};
	EObjectType ObjectType = {};
};

constexpr uint32 ONCE_EXECUTER_LIST_NUM = 100;

BIFROST_NAMESPACE_END