#include "reflect/serial_type_decl.hpp"
#include "reflect/reflection.generated.hpp"

#include <cstdint>
#include <string>
#include <stdexcept>
#include "reflect/container/unique_ptr"
#include "reflect/typeinfo.hpp"
#include "reflect/serialization.hpp"

#include "reflect/utils/assert"

namespace zeno
{
namespace reflect
{

    // Assume always use little-endian

    using IdentifierType = size_t;

    enum class TypeIdentifier : IdentifierType {
        Int32 = 1,
        Float = 2,
        Double = 3,
        Char = 4,
        String = 5,
        Int8 = 6,
        Int16 = 7,
        Int64 = 8,
        Uint8 = 9,
        Uint16 = 10,
        Uint32 = 11,
        Uint64 = 12,
        Float32 = 13,
        Float64 = 14,
        MaxNum,
    };

    class DefaultBinarySerializer : public ISerializer {
    public:
        ISerializer& serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) override {
            if (!input.has_value()) {
                throw std::runtime_error("Cannot serialize a null Any object.");
            }

            const RTTITypeInfo& rtti_info = input.type();

            if (rtti_info == type_info<int>()) {
                serialize_basic_type<int>(out_stream, input, TypeIdentifier::Int32);
            } else if (rtti_info == type_info<float>()) {
                serialize_basic_type<float>(out_stream, input, TypeIdentifier::Float);
            } else if (rtti_info == type_info<double>()) {
                serialize_basic_type<double>(out_stream, input, TypeIdentifier::Double);
            } else if (rtti_info == type_info<char>()) {
                serialize_basic_type<char>(out_stream, input, TypeIdentifier::Char);
            } else if (rtti_info == type_info<std::string>()) {
                serialize_string(out_stream, input);
            } else if (rtti_info == type_info<int8_t>()) {
                serialize_basic_type<int8_t>(out_stream, input, TypeIdentifier::Int8);
            } else if (rtti_info == type_info<int16_t>()) {
                serialize_basic_type<int16_t>(out_stream, input, TypeIdentifier::Int16);
            } else if (rtti_info == type_info<int64_t>()) {
                serialize_basic_type<int64_t>(out_stream, input, TypeIdentifier::Int64);
            } else if (rtti_info == type_info<uint8_t>()) {
                serialize_basic_type<uint8_t>(out_stream, input, TypeIdentifier::Uint8);
            } else if (rtti_info == type_info<uint16_t>()) {
                serialize_basic_type<uint16_t>(out_stream, input, TypeIdentifier::Uint16);
            } else if (rtti_info == type_info<uint32_t>()) {
                serialize_basic_type<uint32_t>(out_stream, input, TypeIdentifier::Uint32);
            } else if (rtti_info == type_info<uint64_t>()) {
                serialize_basic_type<uint64_t>(out_stream, input, TypeIdentifier::Uint64);
            } else {
                throw std::runtime_error("Unsupported type for serialization.");
            }

            return *this;
        }

        ISerializer& deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) override {
            if (!data_stream) {
                throw std::runtime_error("Invalid data stream for deserialization.");
            }

            // Read the type identifier
            IdentifierType identifier = 0;
            data_stream->read(reinterpret_cast<uint8_t*>(&identifier), sizeof(IdentifierType));
            TypeIdentifier type_identifier = static_cast<TypeIdentifier>(identifier);

            switch (type_identifier) {
                case TypeIdentifier::Int32: deserialize_basic_type<int>(data_stream, output); break;
                case TypeIdentifier::Float: deserialize_basic_type<float>(data_stream, output); break;
                case TypeIdentifier::Double: deserialize_basic_type<double>(data_stream, output); break;
                case TypeIdentifier::Char: deserialize_basic_type<char>(data_stream, output); break;
                case TypeIdentifier::String: deserialize_string(data_stream, output); break;
                case TypeIdentifier::Int8: deserialize_basic_type<int8_t>(data_stream, output); break;
                case TypeIdentifier::Int16: deserialize_basic_type<int16_t>(data_stream, output); break;
                case TypeIdentifier::Int64: deserialize_basic_type<int64_t>(data_stream, output); break;
                case TypeIdentifier::Uint8: deserialize_basic_type<uint8_t>(data_stream, output); break;
                case TypeIdentifier::Uint16: deserialize_basic_type<uint16_t>(data_stream, output); break;
                case TypeIdentifier::Uint32: deserialize_basic_type<uint32_t>(data_stream, output); break;
                case TypeIdentifier::Uint64: deserialize_basic_type<uint64_t>(data_stream, output); break;
                default: throw std::runtime_error("Unsupported type for deserialization.");
            }

            return *this;
        }

    private:
        template <typename T>
        void serialize_basic_type(UniquePtr<IWritableStream>& out_stream, const Any& input, TypeIdentifier type_identifier) {
            T value = any_cast<T>(input);
            IdentifierType identifier = IdentifierType(type_identifier);
            out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(IdentifierType));
            out_stream->write(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
        }

        template <typename T>
        void deserialize_basic_type(const UniquePtr<IReadableStream>& data_stream, Any& output) {
            T value;
            data_stream->read(reinterpret_cast<uint8_t*>(&value), sizeof(T));
            output = Any(value);
        }

        void serialize_string(UniquePtr<IWritableStream>& out_stream, const Any& input) {
            std::string& value = any_cast<std::string&>(input);
            IdentifierType identifier = static_cast<IdentifierType>(TypeIdentifier::String);
            out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(IdentifierType));
            uint32_t length = static_cast<uint32_t>(value.size());
            out_stream->write(reinterpret_cast<const uint8_t*>(&length), sizeof(length));
            out_stream->write(reinterpret_cast<const uint8_t*>(value.data()), length);
        }

        void deserialize_string(const UniquePtr<IReadableStream>& data_stream, Any& output) {
            uint32_t length;
            data_stream->read(reinterpret_cast<uint8_t*>(&length), sizeof(length));
            std::string value(length, '\0');
            data_stream->read(reinterpret_cast<uint8_t*>(&value[0]), length);
            output = Any(value);
        }
    };

    UniquePtr<ISerializer> create_default_serializer() {
        return make_unique<DefaultBinarySerializer>();
    }

}
}
