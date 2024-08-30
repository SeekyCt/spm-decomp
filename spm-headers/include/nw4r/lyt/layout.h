#pragma once

#include <common.h>
#include <msl/new.h>
#include <nw4r/lyt/animation.h>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/resourceAccessor.h>

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
    virtual UNKNOWN_FUNCTION(UnbindAnimation);
    virtual UNKNOWN_FUNCTION(UnbindAllAnimation);
    virtual UNKNOWN_FUNCTION(SetAnimationEnable);
    virtual UNKNOWN_FUNCTION(CalculateMtx);
    virtual UNKNOWN_FUNCTION(Draw);
    virtual UNKNOWN_FUNCTION(Animate);
    virtual UNKNOWN_FUNCTION(SetTagProcessor);

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
