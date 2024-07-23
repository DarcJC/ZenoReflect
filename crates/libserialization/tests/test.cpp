
#include "reflect/container/any"
#include "reflect/serialization.hpp"
#include <string>
#include <iostream>

#include "reflect/reflection.generated.hpp"


int main() {

    using namespace zeno::reflect;

    // Create a serializer
    auto serializer = create_default_serializer();

    // Example data
    std::string example_string = "Hello, World!";
    Any example_any = example_string;

    // Serialize to a file
    {
        UniquePtr<IWritableStream> writable_stream = create_file_write_stream("example.dat");
        serializer->serialize(writable_stream, example_any);
    }

    // Deserialize from a file
    {
        UniquePtr<IReadableStream> readable_stream = create_file_read_stream("example.dat");
        Any deserialized_any;
        serializer->deserialize(readable_stream, deserialized_any);

        std::string deserialized_string = any_cast<std::string>(deserialized_any);
        std::cout << "Deserialized value: " << deserialized_string << std::endl;
    }


    return 0;
}
