#pragma once

#include <cstdint>
#include <stdint.h>
#include <string_view>
#include <type_traits>
#include "reflect/container/any"
#include "reflect/container/duck"
#include "reflect/container/unique_ptr"
#include "reflect/polyfill.hpp"
#include "reflect/registry.hpp"
#include "reflect/serial_primary_type.hpp"
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

        virtual bool serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) = 0;
        virtual bool deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) = 0;
    };

    class REFLECT_SERIALIZATION_API Archive : public any::TEnableAnyFromThis<Archive> {
    public:
        virtual ~Archive() = default;

        Archive(UniquePtr<ISerializer> serializer = nullptr, UniquePtr<IReadableStream> read_stream = nullptr, UniquePtr<IWritableStream> write_stream = nullptr);

        Archive& set_flags(uint32_t flags);

        bool has_flags(uint32_t flags);

        template <typename T>
        Archive& archive(T&& serialize_object) {
            if REFLECT_FORCE_CONSTEPXR (std::is_same_v<T, Any>) {
                return *this;
            }

            using DecayT = std::decay_t<T>;

            // TODO Support any from this
            ZENO_CHECK_MSG(has_flags(AF_Out) || IsSerializationPrimaryType<DecayT>::value, "Only serializing is supported of current function.");

            if REFLECT_FORCE_CONSTEPXR (IsSerializationPrimaryType<DecayT>::value) {
                ZENO_CHECK(m_write_stream && m_primary_serializer);
                m_primary_serializer->serialize(m_write_stream, make_any<T>(serialize_object));
            } else {
                archive(make_any<T>(std::forward<T>(serialize_object)));
            }

            return *this;
        }

        Archive& archive_any(Any& serialize_object);

    protected:
        bool try_recursive_archive(Any& serialize_object);

        UniquePtr<ISerializer> m_primary_serializer = nullptr;
        UniquePtr<IReadableStream> m_read_stream = nullptr;
        UniquePtr<IWritableStream> m_write_stream = nullptr;
        
        uint32_t m_flags = AF_None;
    };

    REFLECT_SERIALIZATION_API extern UniquePtr<ISerializer> create_default_serializer();

    REFLECT_SERIALIZATION_API extern UniquePtr<IReadableStream> create_file_read_stream(std::string_view path);

    REFLECT_SERIALIZATION_API extern UniquePtr<IWritableStream> create_file_write_stream(std::string_view path);

    inline REFLECT_FORCE_CONSTEPXR HighOrderCallableType<bool, Archive&> SerializeFunctionDelegate("serialize");

}
}

REFLECT_REGISTER_RTTI_TYPE_MANUAL(zeno::reflect::Archive);
