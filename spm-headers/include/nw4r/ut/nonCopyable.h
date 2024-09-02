/*
    Based on the decompilation in ogws by kiwi515
*/

#pragma once

#include <common.h>
#include <nw4r/ut/nonCopyable.h>

namespace nw4r {
namespace ut {
namespace {

class NonCopyable {
protected:
    NonCopyable() {}
    NonCopyable(const NonCopyable&) {}
    ~NonCopyable() {}
};
SIZE_ASSERT(NonCopyable, 0x1)

} // namespace
} // namespace ut
} // namespace nw4r
