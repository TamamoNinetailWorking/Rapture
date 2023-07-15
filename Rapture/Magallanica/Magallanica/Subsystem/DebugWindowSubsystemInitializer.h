#pragma once

ATLANTIS_NAMESPACE_BEGIN

class CRHIProcessor;

ATLANTIS_NAMESPACE_END

MAGALLANICA_NAMESPACE_BEGIN

struct FDebugWindowSubsystemInitializer
{
	HWND hwnd = {};
	const ATLANTIS_NAMESPACE::CRHIProcessor* Processor = nullptr;
};

MAGALLANICA_NAMESPACE_END