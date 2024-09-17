#pragma once

#include <common.h>

namespace nw4r {
namespace lyt {

class ResourceAccessor
{
public:
    ResourceAccessor();
    virtual ~ResourceAccessor();
    virtual void * GetResource(u32 type, const char * name, u32 * sizeOut);
    virtual UNKNOWN_FUNCTION(GetFont);
};
SIZE_ASSERT(ResourceAccessor, 0x4)

} // namespace lyt
} // namespace nw4r
