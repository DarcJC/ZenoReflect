option(REFLECT_SERIALIZATION_EXTERNAL_INCLUDE_PATH "Add external path to include path. Use it if you modified generator output." "")

add_library(ReflectSerialization SHARED
    src/serialization.cpp
    src/binary_serialization.cpp
)
add_library(Reflect::Serialization ALIAS ReflectSerialization)

target_link_libraries(ReflectSerialization PUBLIC libreflect libgenerated)

target_include_directories(ReflectSerialization PUBLIC
    BEFORE ${REFLECT_SERIALIZATION_EXTERNAL_INCLUDE_PATH} include 
)

make_absolute_paths(reflect_serialization_headers include/reflect/serial_type_decl.hpp)
zeno_declare_reflection_support(ReflectSerialization ${reflect_serialization_headers})
