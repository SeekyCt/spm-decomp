#pragma once

#include <common.h>
#include <wii/mem.h>
#include <msl/new>
#include <nw4r/lyt/animation.hpp>
#include <nw4r/lyt/drawInfo.hpp>
#include <nw4r/lyt/pane.hpp>
#include <nw4r/lyt/resourceAccessor.hpp>
#include <nw4r/ut/rect.hpp>

namespace nw4r {
namespace lyt {

USING(wii::mem::MEMAllocator)

class Layout
{
public:
    Layout();
    virtual ~Layout();

    virtual bool Build(void * lytResBuf, ResourceAccessor * pResAcsr);
    virtual AnimTransform * CreateAnimTransform(void * animResBuf, ResourceAccessor * pResAcsr);
    virtual void BindAnimation(AnimTransform * pAnimTrans);
    virtual void UnbindAnimation(AnimTransform * pAnimTrans);
    virtual UNKNOWN_FUNCTION(UnbindAllAnimation);
    virtual UNKNOWN_FUNCTION(SetAnimationEnable);
    virtual void CalculateMtx(const DrawInfo& drawInfo);
    virtual void Draw(const DrawInfo& drawInfo);
    virtual void Animate(u32 option);
    virtual UNKNOWN_FUNCTION(SetTagProcessor);

    ut::Rect GetLayoutRect();

    static void SetAllocator(MEMAllocator * allocator)
    {
        mspAllocator = allocator;
    }

    Pane * GetRootPane()
    {
        return mRootPane;
    }

    u8 unknown_0x4[0x10 - 0x4];
    Pane * mRootPane;
    u8 unknown_0x14[0x24 - 0x14];

    static MEMAllocator * mspAllocator;
};
SIZE_ASSERT(Layout, 0x24)

} // namespace lyt
} // namespace nw4r
