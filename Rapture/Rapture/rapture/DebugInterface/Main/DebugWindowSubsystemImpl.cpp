#include "DebugWindowSubsystemImpl.h"

#include "../Infrastructure/DebugFrameRate.h"

#define REGIST_DEBUG_WINDOW(param) \
	param = new C##param(); \
	CHECK_RESULT_FALSE(param); \
	RegistDebugWindow(param); \

#define REGIST_DEBUG_WINDOW_PARAM(param) \
	C##param* param = nullptr; \

#define REGIST_DEBUG_WINDOW_PROCESS(param) \
	REGIST_DEBUG_WINDOW_PARAM(param) \
	REGIST_DEBUG_WINDOW(param) \

//CDebugFrameRate* DebugFrameRate = nullptr;

//REGIST_DEBUG_WINDOW_PARAM(DebugFrameRate);


bool CDebugWindowSubsystemImpl::InitializeImpl()
{
	/*DebugFrameRate = new CDebugFrameRate();
	CHECK_RESULT_FALSE(DebugFrameRate);
	RegistDebugWindow(DebugFrameRate);*/



	//REGIST_DEBUG_WINDOW(DebugFrameRate);

	REGIST_DEBUG_WINDOW_PROCESS(DebugFrameRate);

	return true;
}


#undef REGIST_DEBUG_WINDOW
#undef REGIST_DEBUG_WINDOW_PARAM