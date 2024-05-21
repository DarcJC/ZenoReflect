cmake_minimum_required(VERSION 3.26)
project(ZenoReflect LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/FindPackages.cmake)
include(cmake/ReflectionUtils.cmake)

set(RELCTION_GENERATOR_TARGET ReflectGenerator)
add_executable(${RELCTION_GENERATOR_TARGET} 
    src/main.cpp src/args.cpp src/utils.cpp
)

add_executable(ZenoReflect::generator ALIAS ${RELCTION_GENERATOR_TARGET})
target_link_libraries(${RELCTION_GENERATOR_TARGET} ${LIBCLANG_LIBRARY} fmt::fmt)
target_include_directories(${RELCTION_GENERATOR_TARGET} PUBLIC ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

add_subdirectory(crates)
add_subdirectory(example)
