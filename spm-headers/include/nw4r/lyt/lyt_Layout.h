#pragma once

#include <common.h>

namespace nw4r {

namespace lyt {

class Layout
{
public:
    virtual ~Layout();
    virtual UNKNOWN_FUNCTION(Build);
    virtual UNKNOWN_FUNCTION(CreateAnimTransform);
    virtual UNKNOWN_FUNCTION(BindAnimation);
    virtual UNKNOWN_FUNCTION(UnbindAnimation);
    virtual UNKNOWN_FUNCTION(UnbindAllAnimation);
    virtual UNKNOWN_FUNCTION(SetAnimationEnable);
    virtual UNKNOWN_FUNCTION(CalculateMtx);
    virtual UNKNOWN_FUNCTION(Draw);
    virtual UNKNOWN_FUNCTION(Animate);
    virtual UNKNOWN_FUNCTION(SetTagProcessor);

    u8 unknown_0x4[0x24 - 0x4];
};
SIZE_ASSERT(Layout, 0x24)

}

}
