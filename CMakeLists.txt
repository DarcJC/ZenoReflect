cmake_minimum_required(VERSION 3.26)
project(ZenoReflect LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/FindPackages.cmake)
include(cmake/ReflectionUtils.cmake)

set(RELCTION_GENERATOR_TARGET ReflectGenerator)
llvm_map_components_to_libnames(LLVM_LINK_COMPONENTS support)
set(LLVM_ENABLE_RTTI ON)
set(LLVM_ENABLE_EH ON)
add_executable(${RELCTION_GENERATOR_TARGET} 
    src/main.cpp src/args.cpp src/utils.cpp src/parser.cpp src/metadata.cpp
)

add_executable(ZenoReflect::generator ALIAS ${RELCTION_GENERATOR_TARGET})
target_link_libraries(${RELCTION_GENERATOR_TARGET} PRIVATE LLVM ${LIBCLANG_LIBRARY})
target_include_directories(${RELCTION_GENERATOR_TARGET} PUBLIC ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})

add_subdirectory(crates)
add_subdirectory(example)
