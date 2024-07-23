#pragma once

#include <cstdint>
#include <stdint.h>
#include <string_view>
#include "reflect/container/any"
#include "reflect/container/unique_ptr"

#ifdef _MSC_VER
#   ifdef REFLECT_SERIALIZATION_EXPORTS
#       define REFLECT_SERIALIZATION_API __declspec(dllexport)
#   else
#       define REFLECT_SERIALIZATION_API __declspec(dllimport)
#   endif
#else
#   define REFLECT_SERIALIZATION_API __attribute__((visibility("default")))
#endif

namespace zeno
{
namespace reflect
{

    class REFLECT_SERIALIZATION_API IReadableStream {
    public:
        virtual ~IReadableStream() = default;

        virtual void read(uint8_t* buf, size_t len) = 0;

        virtual uint8_t next_byte() = 0;
    };

    class REFLECT_SERIALIZATION_API IWritableStream {
    public:
        virtual ~IWritableStream() = default;

        virtual void write_byte(uint8_t data) = 0;

        virtual void write(const uint8_t* data, size_t len) = 0;
    };

    class REFLECT_SERIALIZATION_API ISerializer {
    public:
        virtual ~ISerializer() = default;

        virtual ISerializer& serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) = 0;
        virtual ISerializer& deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) = 0;
    };

    REFLECT_SERIALIZATION_API extern UniquePtr<ISerializer> create_default_serializer();

    REFLECT_SERIALIZATION_API extern UniquePtr<IReadableStream> create_file_read_stream(std::string_view path);

    REFLECT_SERIALIZATION_API extern UniquePtr<IWritableStream> create_file_write_stream(std::string_view path);

}
}
