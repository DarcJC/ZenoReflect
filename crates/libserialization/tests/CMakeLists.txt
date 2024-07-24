
add_executable(ReflectSerializationTest
    test.cpp
)

target_link_libraries(ReflectSerializationTest PUBLIC Reflect::Serialization)

make_absolute_paths(REFLECT_SERIALIZATION_TEST_HEADERS 
    test.h
)
zeno_declare_reflection_support(ReflectSerializationTest ${REFLECT_SERIALIZATION_TEST_HEADERS})
