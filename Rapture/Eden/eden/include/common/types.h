/////////////////////////////////////////////////////////////////////////////
///@file	types.h
///@brief	type define
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

typedef				char			sint8;
typedef				short			sint16;
typedef				int				sint32;
typedef				long long		sint64;
typedef				float			float32;
typedef				double			float64;

typedef unsigned	char			uint8;
typedef unsigned	short			uint16;
typedef unsigned	int				uint32;
typedef unsigned	long long		uint64;


#define SCast static_cast
#define PCast static_cast
#define DCast dynamic_cast
#define RCast reinterpret_cast
#define CCast const_cast

#define SNumCast SCast<sint32>
#define UNumCast SCast<uint32>

#endif