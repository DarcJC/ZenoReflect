#pragma once

namespace zeno
{
namespace reflect
{
    template <bool Cond, typename T = void>
    struct TEnableIf {};
    

    template <typename T>
    struct TEnableIf<true, T> {
        using Type = T;
    };

    template <bool Cond, typename T = void>
    using TTEnableIf = typename TEnableIf<Cond, T>::Type;
} // namespace reflect 
} // namespace zeno

