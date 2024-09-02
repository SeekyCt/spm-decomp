#pragma once

#include <common.h>

namespace nw4r {
namespace ut {

struct Rect
{
    f32 left, top, bottom, right;
};
SIZE_ASSERT(Rect, 0x10)

} // namespace ut
} // namespace nw4r
