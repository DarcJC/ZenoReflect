#pragma once

#include <cstdint>

namespace zeno
{
namespace reflect
{

    template <typename T>
    struct TRemoveReference {
        using Type = T;
    };

    template <typename T>
    struct TRemoveReference<T&> {
        using Type = T;
    };

    template <typename T>
    struct TRemoveReference<T&&> {
        using Type = T;
    };

    template <typename T>
    using TTRemoveReference = typename TRemoveReference<T>::Type;

    template <typename T>
    T&& forward(TTRemoveReference<T>& v) {
        return static_cast<T&&>(v);
    }

    template <typename T>
    T&& forward(TTRemoveReference<T>&& v) {
        return static_cast<T&&>(v);
    }

    template <typename T>
    struct TRemovePointer {
        using Type = T;
    };

    template <typename T>
    struct TRemovePointer<T*> {
        using Type = T;
    };

    template <typename T>
    using TTRemovePointer = typename TRemovePointer<T>::Type;

    template <typename T>
    struct TRemoveExtent {
        using Type = T;
    };

    template <typename T, size_t N>
    struct TRemoveExtent<T[N]> {
        using Type = T*;
    };

    template <typename T>
    struct TRemoveExtent<T[]> {
        using Type = T*;
    };

    template <typename T>
    struct TRemoveCV {
        using Type = T;
    };

    template <typename T>
    struct TRemoveCV<const T> {
        using Type = T;
    };

    template <typename T>
    struct TRemoveCV<volatile T> {
        using Type = T;
    };

    template <typename T>
    struct TRemoveCV<const volatile T> {
        using Type = T;
    };

    template <typename T>
    struct TDecay {
        using Type = TTRemoveReference<typename TRemoveCV<typename TRemoveExtent<T>::Type>::Type>;
    };

    template <typename T>
    using TTDecay = typename TDecay<T>::Type;

} // namespace reflect 
} // namespace zeno

