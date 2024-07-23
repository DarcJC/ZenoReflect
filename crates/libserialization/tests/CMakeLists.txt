
add_executable(ReflectSerializationTest
    test.cpp
)

target_link_libraries(ReflectSerializationTest PUBLIC Reflect::Serialization)
