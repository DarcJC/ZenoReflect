
add_library(ReflectExample 
    src/example.cpp
)
target_link_libraries(ReflectExample PUBLIC ZenoReflect::libreflect)
target_include_directories(ReflectExample PRIVATE include)

get_target_property(ExampleIncludes ReflectExample INCLUDE_DIRECTORIES)
message(STATUS "${ExampleIncludes}")
