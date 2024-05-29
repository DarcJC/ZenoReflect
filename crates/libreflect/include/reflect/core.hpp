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

#include "registry.hpp"
