#pragma once

// #include "reflect/type"

namespace test {
struct Parent {};
namespace inner {
class Oops {};
union Yeppp {};
}
}

using AliasType1 = test::Parent;
typedef test::Parent AliasType2;
using Yeppp = test::inner::Yeppp;

struct ZSTRUCT(我是你爹=真, 测试=(组1, Group 2, Group 3), 123=true, 456) IAmPrimitve : public AliasType1 {
    signed int i32;

    ZNODE(Name="我日你妈") 
    void DoSomething(int value);
};

