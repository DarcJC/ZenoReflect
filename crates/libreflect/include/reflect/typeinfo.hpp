#pragma once

#include <cstddef>
#include "reflect/macro.hpp"
#include "reflect/polyfill.hpp"
#include "reflect/traits/reference.hpp"

namespace zeno
{
namespace reflect
{
    class RTTITypeInfo;

    class LIBREFLECT_API RTTITypeInfo {
    public:
        // Important: This constructor is internal, don't use it
        REFLECT_CONSTEXPR RTTITypeInfo(const char* in_name, std::size_t hashcode): m_name(in_name), m_hashcode(hashcode) {}
        RTTITypeInfo& operator=(const RTTITypeInfo&&) = delete;

        const char* name() const;
        size_t hash_code() const;

        /**
         * Compare two type info using hash code.
        */
        REFLECT_STATIC_CONSTEXPR bool equal_fast(const RTTITypeInfo& other) const;
        /**
         * Compare two type info based on their address.
         * 
         * This fast path should only be safe if using inside the same translation unit.
        */
        REFLECT_STATIC_CONSTEXPR bool equal_fast_unsafe(const RTTITypeInfo& other) const;
        bool operator==(const RTTITypeInfo& other) const;
        bool operator!=(const RTTITypeInfo& other) const;
    private:

        const char* m_name;
        size_t m_hashcode;
    };

    // SFINAE
    template <typename T>
    REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo Default = {"<default_type>", 0}; 
#ifdef ZENO_REFLECT_PROCESSING
        return Default;
#else
        static_assert(false, "\r\n==== Reflection Error ====\r\nThe type_info of current type not implemented. Have you marked it out ?\r\nTry '#include \"generated_templates.hpp\"' in the traslation unit where you used zeno::reflect::type_info. \r\n==== Reflection Error End ====");
        return Default;
#endif
    }
}
}
