#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include <memory>
#include <atomic>
#include "reflect/macro.hpp"

namespace zeno
{
namespace reflect 
{
    struct internal_utils {
        static LIBREFLECT_API int32_t allocate_new_id();
    };
}
}

