#pragma once

#include <common.h>

namespace nw4r {
namespace db {

#define NW4R_ASSERT_PTR(line, ptr) \
    if (ptr == NULL) nw4r::db::Panic(__FILE__, line, "NW4R:Pointer must not be NULL (" #ptr ")")

void Panic(const char * msg, ...);

}
}
