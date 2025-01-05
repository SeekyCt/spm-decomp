#pragma once

#include <common.h>
#include <wii/gx.h>
#include <nw4r/ut/linkList.hpp>

namespace nw4r {
namespace lyt {

USING(wii::gx::GXColorS10)

class Material
{
public:
    Material();
    virtual ~Material();

    u8 unknown_0x4[0x10 - 0x4];
    GXColorS10 mTevCols[3];
    u8 unknown_0x28[0x5c - 0x28];
};
SIZE_ASSERT(Material, 0x5c)

} // namespace lyt
} // namespace nw4r
