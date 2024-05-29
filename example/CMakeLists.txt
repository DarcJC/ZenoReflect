set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ReflectExample SHARED
    src/example.cpp
)
target_link_libraries(ReflectExample PUBLIC ZenoReflect::libreflect ZenoReflect::libgenerated)
target_include_directories(ReflectExample PRIVATE include)

make_absolute_paths(REFLECTION_HEADERS 
    include/data.h
) 
zeno_declare_reflection_support(ReflectExample ${REFLECTION_HEADERS})
