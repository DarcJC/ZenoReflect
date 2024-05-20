
add_library(libreflect STATIC
    src/reflect.cpp
    src/registry.cpp
)
add_library(ZenoReflect::libreflect ALIAS libreflect)
target_include_directories(libreflect PUBLIC include)

set(PRE_INCLUDE_HEADER "${CMAKE_CURRENT_LIST_DIR}/include/reflect/core.hpp")
target_precompile_headers(libreflect PUBLIC ${PRE_INCLUDE_HEADER})
get_target_property(LIBREFLECT_INCLUDE_DIRS libreflect INCLUDE_DIRECTORIES)
set(LIBREFLECT_INCLUDE_DIRS ${LIBREFLECT_INCLUDE_DIRS} CACHE INTERNAL "LIBREFLECT_INCLUDE_DIRS")
