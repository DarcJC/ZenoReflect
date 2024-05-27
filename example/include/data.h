#pragma once

namespace test {
struct Parent {};
}

// using AliasType = test::Parent;
typedef test::Parent AliasType;

struct ZSTRUCT(我是你爹=真, 测试=(组1, Group 2, Group 3), 123) IAmPrimitve : public AliasType {
    signed int i32;

    ZNODE(Name="我日你妈") 
    void DoSomething(int value);
};

