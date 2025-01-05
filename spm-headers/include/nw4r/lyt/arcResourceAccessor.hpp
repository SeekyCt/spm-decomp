#pragma once

#include <common.h>
#include <nw4r/lyt/resourceAccessor.hpp>
#include <nw4r/ut/linkList.hpp>

namespace nw4r {
namespace lyt {

class FontRefLink
{
public:
    FontRefLink();
    ~FontRefLink() {}

    nw4r::ut::LinkListNode mLink;

    u8 unknown_0x8[0x8c - 0x8];
};
SIZE_ASSERT(FontRefLink, 0x8c)

class ArcResourceAccessor : public ResourceAccessor
{
public:
    ArcResourceAccessor();
    virtual ~ArcResourceAccessor()
    {

    }

    virtual void * GetResource(u32 type, const char * name, u32 * sizeOut);
    virtual UNKNOWN_FUNCTION(GetFont);

    bool Attach(void * file, const char * rootDir);

    u8 unknown_0x4[0x24 - 0x4];
    nw4r::ut::LinkList<FontRefLink, offsetof(FontRefLink, mLink)> mFontList;
    u8 unknown_0x30[0xb0 - 0x30];
};
SIZE_ASSERT(ArcResourceAccessor, 0xb0)

} // namespace lyt
} // namespace nw4r
