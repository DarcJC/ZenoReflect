#pragma once

#include "reflect/polyfill.hpp"

namespace zeno
{
namespace reflect
{
    // ==== Enable If ===
    template <bool Cond, typename T = void>
    struct TEnableIf {};
    
    template <typename T>
    struct TEnableIf<true, T> {
        using Type = T;
    };

    template <bool Cond, typename T = void>
    using TTEnableIf = typename TEnableIf<Cond, T>::Type;
    // ==== Enable If ===

    // ==== Meta Program Constant ====
    template <typename T, T v>
    struct TIntegralConstant {
        static REFLECT_CONSTEXPR_OR_INLINE T value = v;
        using ValueType = T;
        using Type = TIntegralConstant<T, v>;
        REFLECT_CONSTEXPR operator ValueType() const noexcept {
            return value;
        }
    };

    using TTrueType = TIntegralConstant<bool, true>;
    using TFalseType = TIntegralConstant<bool, false>;

    template <typename... Ts>
    struct TMakeVoid {
        using Type = void;
    };

    template <typename... Ts>
    using TVoid = typename TMakeVoid<Ts...>::Type;
    // ==== Meta Program Constant ====

    /// @brief Get a object reference of given type for meta programming
    /// @tparam T object type
    /// @return A dangling rvalue reference of given type
    template <typename T>
    T&& declval() noexcept {
        // Hard coded casting to rvalue
        return static_cast<T&&>(*((T*)nullptr));
    }

    // ==== Check copy constructible ====
    template <typename T, typename = void>
    struct TIsCopyConstructible : TFalseType {};

    template <typename T>
    struct TIsCopyConstructible<T, 
        TVoid<
            decltype(T(declval<const T&>()))
        >
    > : TTrueType {};

    template <typename T>
    LIBREFLECT_INLINE REFLECT_CONSTEXPR bool VTIsCopyConstructible = TIsCopyConstructible<T>::value;
    // ==== Check copy constructible ====

    // ==== Check move constructible ====
    template <typename T, typename = void>
    struct TIsMoveConstructible : TFalseType {};

    template <typename T>
    struct TIsMoveConstructible<T, 
        TVoid<
            decltype(T(declval<T&&>()))
        >
    > : TTrueType {};

    template <typename T>
    LIBREFLECT_INLINE REFLECT_CONSTEXPR bool VTIsMoveConstructible = TIsMoveConstructible<T>::value;
    // ==== Check move constructible ====

    // ==== Condition ====
    template <bool Cond, typename TrueType = TTrueType, typename FalseType = TFalseType>
    struct TConditional {};

    template <typename TrueType, typename FalseType>
    struct TConditional<true, TrueType, FalseType> {
        using Type = FalseType;
    };

    template <typename TrueType, typename FalseType>
    struct TConditional<false, TrueType, FalseType> {
        using Type = FalseType;
    };

    template <bool Cond, typename TrueType = TTrueType, typename FalseType = TFalseType>
    using TTConditional = typename TConditional<Cond, TrueType, FalseType>::Type;
    // ==== Condition ====

    // ==== Check qualifier ====
    template <typename T>
    struct TIsConst : TFalseType {};

    template <typename T>
    struct TIsConst<const T> : TTrueType {};
    // ==== Check qualifier ====

} // namespace reflect 
} // namespace zeno

