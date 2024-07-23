#pragma once

#include <cstdint>
#include <stdint.h>
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
        ~IReadableStream() = default;

        virtual void read(uint8_t* buf, size_t len) = 0;

        virtual uint8_t next_byte() const = 0;
    };

    class REFLECT_SERIALIZATION_API IWritableStream {
    public:
        ~IWritableStream() = default;

        virtual void write_byte(uint8_t data) = 0;

        virtual void write(const uint8_t* data, size_t len) = 0;
    };

    class REFLECT_SERIALIZATION_API ISerializer {
    public:
        ~ISerializer() = default;

        virtual ISerializer& serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) = 0;
        virtual ISerializer& deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) = 0;
    };

    REFLECT_SERIALIZATION_API extern UniquePtr<ISerializer> create_default_serializer();

}
}
