option(REFLECT_SERIALIZATION_EXTERNAL_INCLUDE_PATH "Add external path to include path. Use it if you modified generator output." "")
option(REFLECT_ENABLE_SERIALIZATION_TEST_TARGET "" OFF)

add_library(ReflectSerialization SHARED
    src/serialization.cpp
    src/binary_serialization.cpp
    src/file_stream.cpp
)
add_library(Reflect::Serialization ALIAS ReflectSerialization)

target_link_libraries(ReflectSerialization PUBLIC libreflect libgenerated)

target_include_directories(ReflectSerialization PUBLIC
    BEFORE ${REFLECT_SERIALIZATION_EXTERNAL_INCLUDE_PATH} include 
)

target_compile_definitions(ReflectSerialization PRIVATE REFLECT_SERIALIZATION_EXPORTS=1)

make_absolute_paths(REFLECT_SERIALIZATION_HEADERS 
    include/reflect/serialization.hpp
    include/reflect/serial_type_decl.hpp
)
set(INJA_TEMPLATE_DIR_PATH  ${CMAKE_BINARY_DIR}/intermediate)
zeno_declare_reflection_support(ReflectSerialization "${REFLECT_SERIALIZATION_HEADERS}")

if (REFLECT_ENABLE_SERIALIZATION_TEST_TARGET)
    add_subdirectory(tests)
endif ()
