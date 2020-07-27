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
	va_list arg;
	u32 done;

	va_start(arg, _format);
	done = vfprintf(stdout, _format, arg);
	va_end(arg);

	return done;
}


void __assert(b8 _expr)
{
	assert(_expr);
}


void __assert_msg(b8 _expr, const c8* _format, ...)
{
	va_list arg;
	u32 done;

	va_start(arg, _format);
	done = vfprintf(stdout, _format, arg);
	va_end(arg);

	assert(_expr);
}