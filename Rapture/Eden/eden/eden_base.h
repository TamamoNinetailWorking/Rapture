﻿/////////////////////////////////////////////////////////////////////////////
///@file	eden_base.h
///@brief	Eden Library's Base Rule
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __EDEN_BASE_H__
#define __EDEN_BASE_H__


///Base Rule Define
//--------------------------------------------------------------
#define EDENS_NAMESPACE eden
#define EDENS_NAMESPACE_BEGIN namespace EDENS_NAMESPACE{
#define EDENS_NAMESPACE_END };
#define EDENS_NAMESPACE_USING using namespace EDENS_NAMESPACE;
//--------------------------------------------------------------

EDENS_NAMESPACE_BEGIN
EDENS_NAMESPACE_END


// Base Macro
#define ARRAYOF(_Array) sizeof(_Array) / sizeof(_Array[0])
// CheckPointer normal return
#define CHECK(ptr) if(!ptr) { return; };
// CheckResult if not ,return false
#define CHECK_RESULT_FALSE(result) if(!result) { return false; };
// CheckResult if not , break
#define CHECK_RESULT_BREAK(result) if(!result) { break; };


#endif