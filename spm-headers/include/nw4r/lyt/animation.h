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

    void SetFrame(f32 frame)
    {
        mFrame = frame;
    }

    u16 GetFrameSize() const;

    u8 unknown_0x4[0x10 - 0x4];
    f32 mFrame;
};
SIZE_ASSERT(AnimTransform, 0x14)

}
}
