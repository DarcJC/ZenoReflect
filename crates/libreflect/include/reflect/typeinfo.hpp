#pragma once

#include <cstddef>
#include "polyfill.hpp"

namespace zeno
{
namespace reflect
{
    class RTTITypeInfo;

    class RTTITypeInfo {
    public:
        // Important: This constructor is internal, don't use it
        REFLECT_CONSTEXPR RTTITypeInfo(const char* in_name, std::size_t hashcode): m_name(in_name), m_hashcode(hashcode) {}
        RTTITypeInfo& operator=(const RTTITypeInfo&&) = delete;

        const char* name() const;
        size_t hash_code() const;

    private:

        const char* m_name;
        size_t m_hashcode;
    };

    // SFINAE
    template <typename T>
    REFLECT_CONSTEXPR RTTITypeInfo type_info() {
#ifdef ZENO_REFLECT_PROCESSING
        return RTTITypeInfo{"<default_type>", 0};
#else
        static_assert(false, "The type_info of current type not implemented");
        return RTTITypeInfo{"<default_type>", 0};
#endif
    }
}
}
