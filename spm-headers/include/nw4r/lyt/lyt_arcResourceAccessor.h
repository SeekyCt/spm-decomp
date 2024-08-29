#pragma once

#include <common.h>

namespace nw4r {

namespace lyt {

class ArcResourceAccessor
{
public:
    virtual ~ArcResourceAccessor();
    virtual UNKNOWN_FUNCTION(GetResource);
    virtual UNKNOWN_FUNCTION(GetFont);

    u8 unknown_0x4[0xb0 - 0x4];
};
SIZE_ASSERT(ArcResourceAccessor, 0xb0)

}

}
