/*
    Based on the decompilation in ogws by kiwi515
*/

#pragma once

#include <common.h>

namespace nw4r {
namespace ut {

class NonCopyable {
protected:
    NonCopyable() {}
    NonCopyable(const NonCopyable&) {}
    ~NonCopyable() {}
};
SIZE_ASSERT(NonCopyable, 0x1)

} // namespace ut
} // namespace nw4r
