/*
    Based on the decompilation in ogws by kiwi515
*/

#pragma once

#include <common.h>
#include <wii/mtx.h>

namespace nw4r {
namespace math {

USING(wii::mtx::Vec3)
USING(wii::mtx::Mtx34)

struct _VEC2
{
    f32 x, y;
};

struct VEC2 : _VEC2
{
    VEC2() {}
    VEC2(f32 x_, f32 y_)
    {
        this->x = x_;
        this->y = y_;
    }

    operator f32*() { return reinterpret_cast<f32*>(this); }
    operator const f32*() const { return reinterpret_cast<const f32*>(this); }
};
SIZE_ASSERT(VEC2, 0x8)

struct _VEC3
{
    f32 x, y, z;
};

struct VEC3 : _VEC3
{
    VEC3() {}
    VEC3(f32 x_, f32 y_, f32 z_)
    {
        this->x = x_;
        this->y = y_;
        this->z = z_;
    }

    operator Vec3*() { return reinterpret_cast<Vec3*>(this); }
    operator const Vec3*() const { return reinterpret_cast<const Vec3*>(this); }
};
SIZE_ASSERT(VEC3, 0xc)

struct _MTX34
{
    union
    {
#ifdef DECOMP
        struct
        {
            f32 _00, _01, _02, _03,
                _10, _11, _12, _13,
                _20, _21, _22, _23;
        };
#endif
        f32 m[3][4];
        f32 a[3 * 4];
        Mtx34 mtx;
    };
};

struct MTX34 : public _MTX34
{
    typedef f32 (*MtxRef)[4];
    typedef const f32 (*MtxRefConst)[4];

    MTX34() {}

#ifdef DECOMP
    // clang-format off
    MTX34(f32 f00, f32 f01, f32 f02, f32 f03,
          f32 f10, f32 f11, f32 f12, f32 f13,
          f32 f20, f32 f21, f32 f22, f32 f23) {
        _00 = f00; _01 = f01; _02 = f02; _03 = f03;
        _10 = f10; _11 = f11; _12 = f12; _13 = f13;
        _20 = f20; _21 = f21; _22 = f22; _23 = f23;
    }
    // clang-format on
#endif

    operator MtxRef() { return mtx; }
    operator MtxRefConst() const { return mtx; }
};
SIZE_ASSERT(MTX34, 0x30)

} // namespace math
} // namespace nw4r
