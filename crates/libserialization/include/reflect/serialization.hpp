#pragma once

#include <cstdint>
#include <stdint.h>
#include <string_view>
#include "reflect/container/any"
#include "reflect/container/unique_ptr"
#include "reflect/typeinfo.hpp"
#include "reflect/utils/assert"

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
    enum ArchiveFlag {
        AF_None = 0,
        AF_In = 1,
        AF_Out = 2,
    };

    class REFLECT_SERIALIZATION_API IReadableStream {
    public:
        virtual ~IReadableStream() = default;

        virtual void read(uint8_t* buf, size_t len) = 0;

        virtual uint8_t next_byte() = 0;

        virtual bool is_valid() = 0;

        operator bool() {
            return is_valid();
        }
    };

    class REFLECT_SERIALIZATION_API IWritableStream {
    public:
        virtual ~IWritableStream() = default;

        virtual void write_byte(uint8_t data) = 0;

        virtual void write(const uint8_t* data, size_t len) = 0;

        virtual bool is_valid() = 0;

        operator bool() {
            return is_valid();
        }
    };

    class REFLECT_SERIALIZATION_API ISerializer {
    public:
        virtual ~ISerializer() = default;

        virtual ISerializer& serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) = 0;
        virtual ISerializer& deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) = 0;
    };

    class REFLECT_SERIALIZATION_API Archive {
    public:
        virtual ~Archive() = default;

        Archive(UniquePtr<ISerializer> serializer = nullptr, UniquePtr<IReadableStream> read_stream = nullptr, UniquePtr<IWritableStream> write_stream = nullptr);

        Archive& set_flags(uint32_t flags);

        bool has_flags(uint32_t flags);

        template <typename T>
        Archive& archive(T&& serialize_object) {
            archive(make_any<T>(zeno::reflect::forward(serialize_object)));

            return *this;
        }

        Archive& archive(Any& serialize_object) {
            ZENO_CHECK_MSG(m_serializer, "Serializer shouldn't be empty");

            if (has_flags(AF_In)) {
                ZENO_CHECK_MSG(m_read_stream && m_read_stream->is_valid(), "Read stream doesn't valid")
                m_serializer->deserialize(m_read_stream, serialize_object);
            } else if (has_flags(AF_Out)) {
                ZENO_CHECK_MSG(m_write_stream && m_write_stream->is_valid(), "Write stream doesn't valid")
                m_serializer->serialize(m_write_stream, serialize_object);
            }

            return *this;
        }
        
    protected:
        UniquePtr<ISerializer> m_serializer = nullptr;
        UniquePtr<IReadableStream> m_read_stream = nullptr;
        UniquePtr<IWritableStream> m_write_stream = nullptr;
        
        uint32_t m_flags = AF_None;
    };

    REFLECT_SERIALIZATION_API extern UniquePtr<ISerializer> create_default_serializer();

    REFLECT_SERIALIZATION_API extern UniquePtr<IReadableStream> create_file_read_stream(std::string_view path);

    REFLECT_SERIALIZATION_API extern UniquePtr<IWritableStream> create_file_write_stream(std::string_view path);

}
}
