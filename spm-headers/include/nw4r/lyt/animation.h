/*
    Based on the decompilation in ogws by kiwi515
*/

#pragma once

#include <common.h>

namespace nw4r {
namespace lyt {

struct AnimTransform
{
    virtual ~AnimTransform();

    u8 unknown_0x4[0x14 - 0x4];
};
SIZE_ASSERT(AnimTransform, 0x14)

}
}
