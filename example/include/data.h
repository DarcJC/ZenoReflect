#pragma once

// #include "reflect/type"
#include <limits>
#include <string>
#include "reflect/reflection.generated.hpp"

namespace test {
struct Parent {
    int abc;
};
namespace inner {
struct Oops {
    int field1;
    const char* field2 = "abc";
    bool field3 = true;
};
union Yeppp {};
}
}

using AliasType1 = test::Parent;
typedef test::Parent AliasType2;
using Yeppp = test::inner::Oops;


namespace zeno
{
    struct ZRECORD(测试1=真, 组测试=(组1, Group 2, Group 3), 123=true, 456, DisplayName="我是一个Prim") IAmPrimitve {
        IAmPrimitve(const IAmPrimitve&) = default;

        signed int i32 = 10086;

        std::string s = "测试内容";

        ZMETHOD(Name="做些事") 
        void DoSomething(int value) const;
    };

    struct ZRECORD(DisplayName="你好") 基类测试 : public AliasType2 {
        
        基类测试() = default;

        ZPROPERTY(DisplayName = "Field 1")
        const char* 字段1 = "Hello World";
    };

    class ZRECORD(Test=true) Hhhh {
    public:
        Hhhh() = delete;

        int a1;
        std::string test;

    };
}

