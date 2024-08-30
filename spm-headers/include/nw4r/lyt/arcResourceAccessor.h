#pragma once

#include <common.h>
#include <nw4r/lyt/resourceAccessor.h>

namespace nw4r {
namespace lyt {

class ArcResourceAccessor : public ResourceAccessor
{
public:
    ArcResourceAccessor();
    virtual ~ArcResourceAccessor();
    virtual void * GetResource(u32 type, const char * name, u32 * sizeOut);
    virtual UNKNOWN_FUNCTION(GetFont);

    bool Attach(void * file, const char * rootDir);

    u8 unknown_0x4[0xb0 - 0x4];
};
SIZE_ASSERT(ArcResourceAccessor, 0xb0)

} // namespace lyt
} // namespace nw4r
