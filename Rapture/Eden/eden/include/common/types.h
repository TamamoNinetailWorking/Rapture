/////////////////////////////////////////////////////////////////////////////
///@file	types.h
///@brief	type define
///@data	2020/06/25
////////////////////////////////////////////////////////////////////////////
#ifndef __TYPES_H__
#define __TYPES_H__


//base type define
typedef				char			s8;
typedef				short			s16;
typedef				int				s32;
typedef				long long		s64;
typedef				float			f32;
typedef				double			f64;

typedef unsigned	char			u8;
typedef unsigned	short			u16;
typedef unsigned	int				u32;
typedef unsigned	long long		u64;

typedef				char			c8;

typedef				bool			b8;


#define SCast static_cast
#define PCast static_cast
#define DCast dynamic_cast
#define RCast reinterpret_cast
#define CCast const_cast

#endif