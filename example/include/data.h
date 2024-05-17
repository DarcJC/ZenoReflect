#pragma once

#if !defined(ZENO_REFLECT_PROCESSING)
    #define ZENO_ANNOTATE(...)
    #define ZSTRUCT(...)
    #define ZPROPERTY(...)
    #define ZNODE(...)
#else
    #define ZENO_ANNOTATE(...) [[clang::annotate(__VA_ARGS__)]]
    #define ZSTRUCT(...) ZENO_ANNOTATE("#struct, " #__VA_ARGS__)
    #define ZPROPERTY(...)  ZENO_ANNOTATE("#property, " #__VA_ARGS__)
    #define ZNODE(...)  ZENO_ANNOTATE("#node, " #__VA_ARGS__)
#endif

struct Parent {};

struct ZSTRUCT(我是你爹(真), 123) IAmPrimitve : public Parent {
    signed int i32;

    ZNODE(Name="我日你妈") 
    void DoSomething(int value);
};

