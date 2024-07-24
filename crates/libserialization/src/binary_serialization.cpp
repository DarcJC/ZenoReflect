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

    class DefaultBinarySerializer : public ISerializer {
    public:
        bool serialize(UniquePtr<IWritableStream>& out_stream, const Any& input) override {
            if (!input.has_value()) {
                throw std::runtime_error("Cannot serialize a null Any object.");
            }

            const RTTITypeInfo& rtti_info = input.type();

            if (rtti_info == type_info<int32_t>()) {
                serialize_basic_type<int>(out_stream, input, type_info<int32_t>().hash_code());
            } else if (rtti_info == type_info<float>()) {
                serialize_basic_type<float>(out_stream, input, type_info<float>().hash_code());
            } else if (rtti_info == type_info<double>()) {
                serialize_basic_type<double>(out_stream, input, type_info<double>().hash_code());
            } else if (rtti_info == type_info<char>()) {
                serialize_basic_type<char>(out_stream, input, type_info<char>().hash_code());
            } else if (rtti_info == type_info<std::string>()) {
                serialize_string(out_stream, input);
            } else if (rtti_info == type_info<int8_t>()) {
                serialize_basic_type<int8_t>(out_stream, input, type_info<int8_t>().hash_code());
            } else if (rtti_info == type_info<int16_t>()) {
                serialize_basic_type<int16_t>(out_stream, input, type_info<int16_t>().hash_code());
            } else if (rtti_info == type_info<int64_t>()) {
                serialize_basic_type<int64_t>(out_stream, input, type_info<int32_t>().hash_code());
            } else if (rtti_info == type_info<uint8_t>()) {
                serialize_basic_type<uint8_t>(out_stream, input, type_info<uint8_t>().hash_code());
            } else if (rtti_info == type_info<uint16_t>()) {
                serialize_basic_type<uint16_t>(out_stream, input, type_info<uint16_t>().hash_code());
            } else if (rtti_info == type_info<uint32_t>()) {
                serialize_basic_type<uint32_t>(out_stream, input, type_info<uint32_t>().hash_code());
            } else if (rtti_info == type_info<uint64_t>()) {
                serialize_basic_type<uint64_t>(out_stream, input, type_info<uint64_t>().hash_code());
            } else {
                IdentifierType identifier = rtti_info.hash_code();
                out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(identifier));
                return false;
            }

            return true;
        }

        bool deserialize(const UniquePtr<IReadableStream>& data_stream, Any& output) override {
            if (!data_stream) {
                throw std::runtime_error("Invalid data stream for deserialization.");
            }

            // Read the type identifier
            IdentifierType type_identifier = 0;
            data_stream->read(reinterpret_cast<uint8_t*>(&type_identifier), sizeof(IdentifierType));

            if (type_identifier == type_info<float>().hash_code()) {
                deserialize_basic_type<float>(data_stream, output);
            } else if (type_identifier == type_info<double>().hash_code()) {
                deserialize_basic_type<double>(data_stream, output);
            } else if (type_identifier == type_info<char>().hash_code()) {
                deserialize_basic_type<char>(data_stream, output);
            } else if (type_identifier == type_info<std::string>().hash_code()) {
                deserialize_string(data_stream, output);
            } else if (type_identifier == type_info<int8_t>().hash_code()) {
                deserialize_basic_type<int8_t>(data_stream, output);
            } else if (type_identifier == type_info<int16_t>().hash_code()) {
                deserialize_basic_type<int16_t>(data_stream, output);
            } else if (type_identifier == type_info<int32_t>().hash_code()) {
                deserialize_basic_type<int32_t>(data_stream, output);
            } else if (type_identifier == type_info<int64_t>().hash_code()) {
                deserialize_basic_type<int64_t>(data_stream, output);
            } else if (type_identifier == type_info<uint8_t>().hash_code()) {
                deserialize_basic_type<uint8_t>(data_stream, output);
            } else if (type_identifier == type_info<uint16_t>().hash_code()) {
                deserialize_basic_type<uint16_t>(data_stream, output);
            } else if (type_identifier == type_info<uint32_t>().hash_code()) {
                deserialize_basic_type<uint32_t>(data_stream, output);
            } else if (type_identifier == type_info<uint64_t>().hash_code()) {
                deserialize_basic_type<uint64_t>(data_stream, output);
            } else {
                return false;
            }

            return true;
        }

        bool serialize(UniquePtr<IWritableStream>& out_stream, const TypeHandle reflect_type, const void* input) override {
            if (reflect_type == type_info<int32_t>()) {
                serialize_basic_type<int>(out_stream, input, type_info<int32_t>().hash_code());
            } else if (reflect_type == type_info<float>()) {
                serialize_basic_type<float>(out_stream, input, type_info<float>().hash_code());
            } else if (reflect_type == type_info<double>()) {
                serialize_basic_type<double>(out_stream, input, type_info<double>().hash_code());
            } else if (reflect_type == type_info<char>()) {
                serialize_basic_type<char>(out_stream, input, type_info<char>().hash_code());
            } else if (reflect_type == type_info<std::string>()) {
                serialize_string(out_stream, static_cast<const std::string*>(input));
            } else if (reflect_type == type_info<int8_t>()) {
                serialize_basic_type<int8_t>(out_stream, input, type_info<int8_t>().hash_code());
            } else if (reflect_type == type_info<int16_t>()) {
                serialize_basic_type<int16_t>(out_stream, input, type_info<int16_t>().hash_code());
            } else if (reflect_type == type_info<int64_t>()) {
                serialize_basic_type<int64_t>(out_stream, input, type_info<int32_t>().hash_code());
            } else if (reflect_type == type_info<uint8_t>()) {
                serialize_basic_type<uint8_t>(out_stream, input, type_info<uint8_t>().hash_code());
            } else if (reflect_type == type_info<uint16_t>()) {
                serialize_basic_type<uint16_t>(out_stream, input, type_info<uint16_t>().hash_code());
            } else if (reflect_type == type_info<uint32_t>()) {
                serialize_basic_type<uint32_t>(out_stream, input, type_info<uint32_t>().hash_code());
            } else if (reflect_type == type_info<uint64_t>()) {
                serialize_basic_type<uint64_t>(out_stream, input, type_info<uint64_t>().hash_code());
            } else {
                IdentifierType identifier = reflect_type.type_hash();
                out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(identifier));
                return false;
            }

            return true;
        }

        bool deserialize(UniquePtr<IReadableStream>& data_stream, const TypeHandle reflect_type, void* output) override {
            // Read the type identifier
            IdentifierType type_identifier = 0;
            data_stream->read(reinterpret_cast<uint8_t*>(&type_identifier), sizeof(IdentifierType));

            ZENO_CHECK_MSG(type_identifier == reflect_type.type_hash(), "Data tag is not matching");

            if (type_identifier == type_info<float>().hash_code()) {
                deserialize_basic_type<float>(data_stream, output);
            } else if (type_identifier == type_info<double>().hash_code()) {
                deserialize_basic_type<double>(data_stream, output);
            } else if (type_identifier == type_info<char>().hash_code()) {
                deserialize_basic_type<char>(data_stream, output);
            } else if (type_identifier == type_info<std::string>().hash_code()) {
                deserialize_string(data_stream, static_cast<std::string*>(output));
            } else if (type_identifier == type_info<int8_t>().hash_code()) {
                deserialize_basic_type<int8_t>(data_stream, output);
            } else if (type_identifier == type_info<int16_t>().hash_code()) {
                deserialize_basic_type<int16_t>(data_stream, output);
            } else if (type_identifier == type_info<int32_t>().hash_code()) {
                deserialize_basic_type<int32_t>(data_stream, output);
            } else if (type_identifier == type_info<int64_t>().hash_code()) {
                deserialize_basic_type<int64_t>(data_stream, output);
            } else if (type_identifier == type_info<uint8_t>().hash_code()) {
                deserialize_basic_type<uint8_t>(data_stream, output);
            } else if (type_identifier == type_info<uint16_t>().hash_code()) {
                deserialize_basic_type<uint16_t>(data_stream, output);
            } else if (type_identifier == type_info<uint32_t>().hash_code()) {
                deserialize_basic_type<uint32_t>(data_stream, output);
            } else if (type_identifier == type_info<uint64_t>().hash_code()) {
                deserialize_basic_type<uint64_t>(data_stream, output);
            } else {
                return false;
            }

            return true;
        }

    private:
        template <typename T>
        bool serialize_basic_type(UniquePtr<IWritableStream>& out_stream, const void* input, IdentifierType type_identifier) {
            ZENO_CHECK(input != nullptr);

            out_stream->write(reinterpret_cast<const uint8_t*>(&type_identifier), sizeof(IdentifierType));
            out_stream->write(reinterpret_cast<const uint8_t*>(input), sizeof(T));

            return true;
        }

        template <typename T>
        bool serialize_basic_type(UniquePtr<IWritableStream>& out_stream, const Any& input, IdentifierType type_identifier) {
            T value = any_cast<T>(input);
            out_stream->write(reinterpret_cast<const uint8_t*>(&type_identifier), sizeof(IdentifierType));
            out_stream->write(reinterpret_cast<const uint8_t*>(&value), sizeof(T));

            return true;
        }

        template <typename T>
        bool deserialize_basic_type(const UniquePtr<IReadableStream>& data_stream, Any& output) {
            T value;
            data_stream->read(reinterpret_cast<uint8_t*>(&value), sizeof(T));
            output = Any(value);

            return true;
        }

        template <typename T>
        bool deserialize_basic_type(const UniquePtr<IReadableStream>& data_stream, void* output) {
            ZENO_CHECK(output != nullptr);

            data_stream->read(reinterpret_cast<uint8_t*>(output), sizeof(T));

            return true;
        }

        bool serialize_string(UniquePtr<IWritableStream>& out_stream, const Any& input) {
            std::string& value = any_cast<std::string&>(input);
            IdentifierType identifier = type_info<std::string>().hash_code();
            out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(IdentifierType));
            uint32_t length = static_cast<uint32_t>(value.size());
            out_stream->write(reinterpret_cast<const uint8_t*>(&length), sizeof(length));
            out_stream->write(reinterpret_cast<const uint8_t*>(value.data()), length);

            return true;
        }

        bool serialize_string(UniquePtr<IWritableStream>& out_stream, const std::string* input) {
            ZENO_CHECK(input != nullptr);
            const std::string& value = *input;
            IdentifierType identifier = type_info<std::string>().hash_code();
            out_stream->write(reinterpret_cast<const uint8_t*>(&identifier), sizeof(IdentifierType));
            uint32_t length = static_cast<uint32_t>(value.size());
            out_stream->write(reinterpret_cast<const uint8_t*>(&length), sizeof(length));
            out_stream->write(reinterpret_cast<const uint8_t*>(value.data()), length);

            return true;
        }

        bool deserialize_string(const UniquePtr<IReadableStream>& data_stream, Any& output) {
            uint32_t length;
            data_stream->read(reinterpret_cast<uint8_t*>(&length), sizeof(length));
            std::string value(length, '\0');
            data_stream->read(reinterpret_cast<uint8_t*>(value.data()), length);
            output = Any(value);

            return true;
        }

        bool deserialize_string(const UniquePtr<IReadableStream>& data_stream, std::string* output) {
            ZENO_CHECK(output != nullptr);

            uint32_t length;
            data_stream->read(reinterpret_cast<uint8_t*>(&length), sizeof(length));
            output->resize(length);
            data_stream->read(reinterpret_cast<uint8_t*>(output->data()), length);

            return true;
        }
    };

    UniquePtr<ISerializer> create_default_serializer() {
        return make_unique<DefaultBinarySerializer>();
    }

}
}
