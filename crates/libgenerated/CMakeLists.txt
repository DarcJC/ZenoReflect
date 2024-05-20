
add_library(libgenerated SHARED
    src/generated_implementations.cpp
)
add_library(ZenoReflect::libgenerated ALIAS libgenerated)
target_precompile_headers(libgenerated PUBLIC include/generated_templates.hpp)
target_include_directories(libgenerated PUBLIC include)
