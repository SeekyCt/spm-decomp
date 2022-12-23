#pragma once

#include <common.h>

namespace ogc::GX {

typedef union _wgpipe
{
    volatile u8 U8;
    volatile s8 S8;
    volatile u16 U16;
    volatile s16 S16;
    volatile u32 U32;
    volatile s32 S32;
    volatile f32 F32;
} WGPipe;

#define wgPipe ((WGPipe *)0xCC008000)

static inline void GX_Position3f32(f32 x,f32 y,f32 z)
{
	wgPipe->F32 = x;
	wgPipe->F32 = y;
	wgPipe->F32 = z;
}

static inline void GX_Position3u16(u16 x,u16 y,u16 z)
{
	wgPipe->U16 = x;
	wgPipe->U16 = y;
	wgPipe->U16 = z;
}

static inline void GX_Position3s16(s16 x,s16 y,s16 z)
{
	wgPipe->S16 = x;
	wgPipe->S16 = y;
	wgPipe->S16 = z;
}

static inline void GX_Position3u8(u8 x,u8 y,u8 z)
{
	wgPipe->U8 = x;
	wgPipe->U8 = y;
	wgPipe->U8 = z;
}

static inline void GX_Position3s8(s8 x,s8 y,s8 z)
{
	wgPipe->S8 = x;
	wgPipe->S8 = y;
	wgPipe->S8 = z;
}

static inline void GX_Position2f32(f32 x,f32 y)
{
	wgPipe->F32 = x;
	wgPipe->F32 = y;
}

static inline void GX_Position2u16(u16 x,u16 y)
{
	wgPipe->U16 = x;
	wgPipe->U16 = y;
}

static inline void GX_Position2s16(s16 x,s16 y)
{
	wgPipe->S16 = x;
	wgPipe->S16 = y;
}

static inline void GX_Position2u8(u8 x,u8 y)
{
	wgPipe->U8 = x;
	wgPipe->U8 = y;
}

static inline void GX_Position2s8(s8 x,s8 y)
{
	wgPipe->S8 = x;
	wgPipe->S8 = y;
}

static inline void GX_Position1x8(u8 index)
{
	wgPipe->U8 = index;
}

static inline void GX_Position1x16(u16 index)
{
	wgPipe->U16 = index;
}

static inline void GX_Normal3f32(f32 nx,f32 ny,f32 nz)
{
	wgPipe->F32 = nx;
	wgPipe->F32 = ny;
	wgPipe->F32 = nz;
}

static inline void GX_Normal3s16(s16 nx,s16 ny,s16 nz)
{
	wgPipe->S16 = nx;
	wgPipe->S16 = ny;
	wgPipe->S16 = nz;
}

static inline void GX_Normal3s8(s8 nx,s8 ny,s8 nz)
{
	wgPipe->S8 = nx;
	wgPipe->S8 = ny;
	wgPipe->S8 = nz;
}

static inline void GX_Normal1x8(u8 index)
{
	wgPipe->U8 = index;
}

static inline void GX_Normal1x16(u16 index)
{
	wgPipe->U16 = index;
}

static inline void GX_Color4u8(u8 r,u8 g,u8 b,u8 a)
{
	wgPipe->U8 = r;
	wgPipe->U8 = g;
	wgPipe->U8 = b;
	wgPipe->U8 = a;
}

static inline void GX_Color3u8(u8 r,u8 g,u8 b)
{
	wgPipe->U8 = r;
	wgPipe->U8 = g;
	wgPipe->U8 = b;
}

static inline void GX_Color3f32(f32 r, f32 g, f32 b)
{
	wgPipe->U8 = (u8)(r * 255.0);
	wgPipe->U8 = (u8)(g * 255.0);
	wgPipe->U8 = (u8)(b * 255.0);
}

static inline void GX_Color1u32(u32 clr)
{
	wgPipe->U32 = clr;
}

static inline void GX_Color1u16(u16 clr)
{
	wgPipe->U16 = clr;
}

static inline void GX_Color1x8(u8 index)
{
	wgPipe->U8 = index;
}

static inline void GX_Color1x16(u16 index)
{
	wgPipe->U16 = index;
}

static inline void GX_TexCoord2f32(f32 s,f32 t)
{
	wgPipe->F32 = s;
	wgPipe->F32 = t;
}

static inline void GX_TexCoord2u16(u16 s,u16 t)
{
	wgPipe->U16 = s;
	wgPipe->U16 = t;
}

static inline void GX_TexCoord2s16(s16 s,s16 t)
{
	wgPipe->S16 = s;
	wgPipe->S16 = t;
}

static inline void GX_TexCoord2u8(u8 s,u8 t)
{
	wgPipe->U8 = s;
	wgPipe->U8 = t;
}

static inline void GX_TexCoord2s8(s8 s,s8 t)
{
	wgPipe->S8 = s;
	wgPipe->S8 = t;
}

static inline void GX_TexCoord1f32(f32 s)
{
	wgPipe->F32 = s;
}

static inline void GX_TexCoord1u16(u16 s)
{
	wgPipe->U16 = s;
}

static inline void GX_TexCoord1s16(s16 s)
{
	wgPipe->S16 = s;
}

static inline void GX_TexCoord1u8(u8 s)
{
	wgPipe->U8 = s;
}

static inline void GX_TexCoord1s8(s8 s)
{
	wgPipe->S8 = s;
}

static inline void GX_TexCoord1x8(u8 index)
{
	wgPipe->U8 = index;
}

static inline void GX_TexCoord1x16(u16 index)
{
	wgPipe->U16 = index;
}

static inline void GX_MatrixIndex1x8(u8 index)
{
	wgPipe->U8 = index;
}

}
