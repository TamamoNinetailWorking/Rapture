/////////////////////////////////////////////////////////////////////////////
///@file	print.h
///@brief	debug print define
///@data	2020/07/11
////////////////////////////////////////////////////////////////////////////
#ifndef __PRINT_H__
#define __PRINT_H__


//proto type
u32 __eden_printf(const c8* _format, ...);
void __assert(b8 _expr);
void __assert_msg(b8 _expr, const c8* _format, ...);


//macro define
#define PRINT(_format,...) __eden_printf(_format,__VA_ARGS__)
#define Assert(expr) __assert(expr)
#define AssertMsg(expr,_format,...) __assert_msg(expr,_format,__VA_ARGS__)

#endif