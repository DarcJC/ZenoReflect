set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ReflectExample 
    src/example.cpp
)
target_link_libraries(ReflectExample PUBLIC ZenoReflect::libreflect)
target_include_directories(ReflectExample PRIVATE include)

get_target_property(ExampleIncludes ReflectExample INCLUDE_DIRECTORIES)
message(STATUS "${ExampleIncludes}")
