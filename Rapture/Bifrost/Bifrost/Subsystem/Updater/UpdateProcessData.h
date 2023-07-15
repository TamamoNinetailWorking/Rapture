#pragma once

#include <list>
#include "UpdateObjectType.h"

EDENS_NAMESPACE_BEGIN

class CObject;

EDENS_NAMESPACE_END


BIFROST_NAMESPACE_BEGIN

typedef std::function<bool(float)> FUpdateFunction;

struct FUpdateProcessData
{
	ObjectPtr(EDENS_NAMESPACE::CObject) ObjectPtr = nullptr;
	FUpdateFunction Function = {};
	EObjectType ObjectType = {};
};

struct FUpdateProcessorHandle
{
private:

	friend class CUpdateProcessor;

	using ProcessorHandle = std::list<FUpdateProcessData*>::iterator;

	ProcessorHandle Handle = {};
	bool IsActive = false;
};

BIFROST_NAMESPACE_END