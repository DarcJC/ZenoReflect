#pragma once

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

} // namespace reflect 
} // namespace zeno

