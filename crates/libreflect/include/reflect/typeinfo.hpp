#pragma once

#include <cstddef>
#include "reflect/macro.hpp"
#include "reflect/polyfill.hpp"
#include "reflect/traits/constant_eval.hpp"
#include "reflect/utils/assert"
#include "reflect/traits/type_traits"

namespace zeno
{
namespace reflect
{
    class RTTITypeInfo;

    enum TypeFlag {
        TF_None = 0,
        TF_IsPointer = 1 << 0,
        TF_IsRValueRef = 1 << 1,
        TF_IsLValueRef = 1 << 2,
        TF_IsConst = 1 << 3,
    };

    template <typename T>
    struct AlwaysFalse : TFalseType {};

    class LIBREFLECT_API RTTITypeInfo {
    public:
        // Important: This constructor is internal, don't use it
        REFLECT_CONSTEXPR RTTITypeInfo(const char* in_name, std::size_t hashcode, size_t flags, size_t decayed_hash = 0) : m_name(in_name), m_hashcode(hashcode), m_flags(flags), m_decayed_hash(decayed_hash) {}

        RTTITypeInfo(const RTTITypeInfo& other);
        RTTITypeInfo(RTTITypeInfo&& other);
        RTTITypeInfo& operator=(const RTTITypeInfo& other);
        RTTITypeInfo& operator=(RTTITypeInfo&& other);

        const char* name() const;
        size_t hash_code() const;
        size_t flags() const;
        bool has_flags(size_t in_flags) const;

        const size_t get_decayed_hash() const;

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
        size_t m_flags;
        size_t m_decayed_hash = 0;
    };

    // SFINAE
    template <typename T>
    static REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo Default = {"<default_type>", 0, 0 }; 
#ifdef ZENO_REFLECT_PROCESSING
        return Default;
#else

        // IDK why some old MSVC will instantiate this template in libreflect
        static_assert(AlwaysFalse<T>::value, "\r\n==== Reflection Error ====\r\nThe type_info of current type not implemented. Have you marked it out ?\r\nTry '#include \"reflect/reflection.generated.hpp\"' in the traslation unit where you used zeno::reflect::type_info. \r\n==== Reflection Error End ====");
        return Default;
#endif
    }

    // We need to instantiate type_info<void> here for Any
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<decltype(nullptr)>() {
        static RTTITypeInfo NullPtr = { "nullptr", 3ULL, 0 };
        return NullPtr;
    }
}
}

///////////////////////////
/// Begin RTTI of "void"
#ifndef _REFLECT_RTTI_GUARD_void_3563412735833858527
#define _REFLECT_RTTI_GUARD_void_3563412735833858527 1

namespace zeno
{
namespace reflect
{
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<void>() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo s = {
            "void",
            3563412735833858527ULL,
            static_cast<size_t>(
                TF_None ),
            0
        };
        return s;
    }
}
}
#endif // _REFLECT_RTTI_GUARD_void_3563412735833858527
/// End RTTI of "void"
///////////////////////////

///////////////////////////
/// Begin RTTI of "class zeno::reflect::Any"
#ifndef _REFLECT_RTTI_GUARD_class_zeno_reflect_Any_15554020952442124146
#define _REFLECT_RTTI_GUARD_class_zeno_reflect_Any_15554020952442124146 1
namespace zeno {
namespace reflect {
class Any;
}
}

namespace zeno
{
namespace reflect
{
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<class zeno::reflect::Any>() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo s = {
            "class zeno::reflect::Any",
            15554020952442124146ULL,
            static_cast<size_t>(
                TF_None ),
            0
        };
        return s;
    }
}
}
#endif // _REFLECT_RTTI_GUARD_class_zeno_reflect_Any_15554020952442124146
/// End RTTI of "class zeno::reflect::Any"
///////////////////////////

///////////////////////////
/// Begin RTTI of "const void *"
#ifndef _REFLECT_RTTI_GUARD_const_void_Mul_9800437855833908128
#define _REFLECT_RTTI_GUARD_const_void_Mul_9800437855833908128 1

namespace zeno
{
namespace reflect
{
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<const void *>() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo s = {
            "const void *",
            9800437855833908128ULL,
            static_cast<size_t>(
                TF_IsPointer | TF_None ),
            type_info<void>().hash_code()
        };
        return s;
    }
}
}
#endif // _REFLECT_RTTI_GUARD_const_void_Mul_9800437855833908128
/// End RTTI of "const void *"
///////////////////////////

///////////////////////////
/// Begin RTTI of "void *"
#ifndef _REFLECT_RTTI_GUARD_void_Mul_14182246238469061381
#define _REFLECT_RTTI_GUARD_void_Mul_14182246238469061381 1

namespace zeno
{
namespace reflect
{
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<void *>() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo s = {
            "void *",
            14182246238469061381ULL,
            static_cast<size_t>(
                TF_IsPointer | TF_None ),
            type_info<void>().hash_code()
        };
        return s;
    }
}
}
#endif // _REFLECT_RTTI_GUARD_void_Mul_14182246238469061381
/// End RTTI of "void *"
///////////////////////////

///////////////////////////
/// Begin RTTI of "const char *"
#ifndef _REFLECT_RTTI_GUARD_const_char_Mul_1226968636088196134
#define _REFLECT_RTTI_GUARD_const_char_Mul_1226968636088196134 1

namespace zeno
{
namespace reflect
{
    template <>
    inline REFLECT_STATIC_CONSTEXPR const RTTITypeInfo& type_info<const char *>() {
        static REFLECT_STATIC_CONSTEXPR RTTITypeInfo s = {
            "const char *",
            1226968636088196134ULL,
            static_cast<size_t>(
                TF_IsPointer | TF_None )
        };
        return s;
    }
}
}
#endif // _REFLECT_RTTI_GUARD_const_char_Mul_1226968636088196134
/// End RTTI of "const char *"
///////////////////////////
