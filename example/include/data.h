#pragma once

namespace test {
struct Parent {};
}

// using AliasType = test::Parent;
typedef test::Parent AliasType;

struct ZSTRUCT(我是你爹(真), 123) IAmPrimitve : public AliasType {
    signed int i32;

    ZNODE(Name="我日你妈") 
    void DoSomething(int value);
};

