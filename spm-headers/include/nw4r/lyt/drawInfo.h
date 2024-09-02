#pragma once

#include <common.h>
#include <nw4r/math/types.h>
#include <nw4r/ut/rect.h>

namespace nw4r {
namespace lyt {

class DrawInfo
{
public:
    virtual ~DrawInfo();
    DrawInfo();

    math::MTX34 mViewMtx;
    ut::Rect mViewRect;
    math::VEC2 mLocationAdjustScale;
    f32 mGlobalAlpha;
    u8 mFlag;
};
SIZE_ASSERT(DrawInfo, 0x54)

} // namespace lyt
} // namespace nw4r
