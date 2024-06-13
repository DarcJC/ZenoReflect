#pragma once

// #include "reflect/type"
#include "reflect/reflection.generated.hpp"

namespace test {
struct Parent {};
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
    struct ZSTRUCT(我是你爹=真, 测试=(组1, Group 2, Group 3), 123=true, 456) IAmPrimitve : public AliasType1 {
        IAmPrimitve(const IAmPrimitve&) = default;

        signed int i32;

        ZNODE(Name="我日你妈") 
        void DoSomething(int value);
    };
}

