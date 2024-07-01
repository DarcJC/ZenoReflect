option(REFLECT_BUILD_EXAMPLE "Build reflection example target" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (REFLECT_BUILD_EXAMPLE)

    add_executable(ReflectExample
        src/example.cpp
    )
    target_include_directories(ReflectExample PRIVATE include)

    make_absolute_paths(REFLECTION_HEADERS 
        include/data.h
        include/test.h
    ) 
    zeno_declare_reflection_support(ReflectExample "${REFLECTION_HEADERS}")

endif()
