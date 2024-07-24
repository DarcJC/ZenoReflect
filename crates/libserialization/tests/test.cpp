
#include "reflect/container/any"
#include "reflect/serialization.hpp"
#include <string>
#include <iostream>
#include <utility>
#include "test.h"


int main() {

    using namespace zeno::reflect;

    // // Create a serializer
    // auto serializer = create_default_serializer();

    // // Example data
    // std::string example_string = "Hello, World!";
    // Any example_any = example_string;

    // // Serialize to a file
    // {
    //     UniquePtr<IWritableStream> writable_stream = create_file_write_stream("example.zarchive");
    //     serializer->serialize(writable_stream, example_any);
    // }

    // // Deserialize from a file
    // if (1)
    // {
    //     UniquePtr<IReadableStream> readable_stream = create_file_read_stream("example.zarchive");
    //     Any deserialized_any;
    //     serializer->deserialize(readable_stream, deserialized_any);

    //     std::string deserialized_string = any_cast<std::string>(deserialized_any);
    //     std::cout << "Deserialized value: " << deserialized_string << std::endl;
    // }

    // How are you?
    {
        HowAreYou hru;
        UniquePtr<IWritableStream> writable_stream = create_file_write_stream("how_are_you.zarchive");
        Archive archive(create_default_serializer() , nullptr, std::move(writable_stream));
        Any a = hru.to_any();
        archive
            .set_flags(AF_Out)
            .archive_any(a);
    }

    return 0;
}
