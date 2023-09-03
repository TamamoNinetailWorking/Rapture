/////////////////////////////////////////////////////////////////////////////
///@file	print.cpp
///@brief	debug print implementation
///@data	2020/07/11
////////////////////////////////////////////////////////////////////////////

#include <stdio.h.>
#include <stdarg.h>
#include <assert.h>

u32 __eden_printf(const c8* _format, ...)
{
	u32 done = 0;
//#ifdef EDENS_DEBUG
	va_list arg;


	va_start(arg, _format);
	done = vfprintf(stdout, _format, arg);
	va_end(arg);


//#endif
	return done;
}


void __assert(b8 _expr)
{
#ifdef EDENS_DEBUG
	assert(_expr);
#endif
}


void __assert_msg(b8 _expr, const c8* _format, ...)
{
#ifdef EDENS_DEBUG
	va_list arg;
	u32 done;

	va_start(arg, _format);
	done = vfprintf(stdout, _format, arg);
	va_end(arg);

	assert(_expr);
#endif
}