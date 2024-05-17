#pragma once
//#include "reflect/core.hpp"

struct Parent {};

struct ZSTRUCT(我是你爹(真), 123) IAmPrimitve : public Parent {
    signed int i32;

    ZNODE(Name="我日你妈") 
    void DoSomething(int value);
};

