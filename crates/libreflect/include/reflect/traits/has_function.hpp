#pragma once

#include <type_traits>
namespace zeno
{
namespace reflect
{

    // 主模板定义，默认情况下是不存在该函数
    template<typename T, typename FuncSig, template<typename> class FuncName, typename = void>
    struct THasFunction : std::false_type {};

    // 专门化版本，使用 decltype 和 SFINAE 检查成员函数
    template<typename T, typename Ret, typename... Args, template<typename> class FuncName>
    struct THasFunction<T, Ret(Args...), FuncName, std::enable_if_t<std::is_class<T>::value && std::is_member_function_pointer<decltype(&T::to_any)>::value, void>>
        : std::true_type {};

    // 函数名模板
    template<typename T>
    struct FunctionName_to_any {
        static constexpr auto value = &T::to_any;
    };

}
}
