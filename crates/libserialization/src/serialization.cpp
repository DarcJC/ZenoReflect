#include <vector>
#include <string>
#include "reflect/metadata.hpp"
#include "reflect/reflection.generated.hpp"

#include "reflect/serialization.hpp"
#include "reflect/container/arraylist"
#include "reflect/utils/assert"

using namespace zeno::reflect;

Archive::Archive(UniquePtr<ISerializer> serializer/* = nullptr*/, UniquePtr<IReadableStream> read_stream/* = nullptr*/, UniquePtr<IWritableStream> write_stream/* = nullptr*/)
    : m_primary_serializer(std::move(serializer))
    , m_read_stream(std::move(read_stream))
    , m_write_stream(std::move(write_stream))
{}

Archive& Archive::set_flags(uint32_t flags) {
    m_flags = flags;
    return *this;
}

bool Archive::has_flags(uint32_t flags) {
    return m_flags & flags;
}

Archive& Archive::archive_fields(TypeBase* reflect_type, void* object) {
    ZENO_CHECK(reflect_type != nullptr);
    ZENO_CHECK(object != nullptr);

    const ArrayList<IMemberField*>& fields = reflect_type->get_member_fields();
    for (const IMemberField* field : fields) {
        ZENO_CHECK(field != nullptr);

        const IRawMetadata* field_metadata = field->get_metadata();
        if (field_metadata->get_value("Transient") != nullptr) {
            continue;
        }

        void* field_ptr = field->get_field_ptr_directly(object);
        ZENO_CHECK(field_ptr != nullptr);
        archive_ptr(field->get_field_type(), field_ptr);
    }

    return *this;
}

Archive& Archive::archive_fields(TypeBase* reflect_type, Any& object) {
    ZENO_CHECK(reflect_type != nullptr);
    ZENO_CHECK(object.type().hash_code() == reflect_type->type_hash());

    const ArrayList<IMemberField*>& fields = reflect_type->get_member_fields();
    for (const IMemberField* field : fields) {
        ZENO_CHECK(field != nullptr);

        const IRawMetadata* field_metadata = field->get_metadata();
        if (field_metadata && field_metadata->get_value("Transient") != nullptr) {
            continue;
        }

        void* field_ptr = field->get_field_ptr(object);
        ZENO_CHECK(field_ptr != nullptr);
        archive_ptr(field->get_field_type(), field_ptr);
    }

    return *this;
}

Archive& Archive::archive_ptr(TypeHandle reflect_type, void* value) {
    ZENO_CHECK_MSG(m_primary_serializer, "Primary serializer shouldn't be empty");

    if (has_flags(AF_In)) {
        ZENO_CHECK_MSG(m_read_stream && m_read_stream->is_valid(), "Read stream is invalid")
        if (!m_primary_serializer->deserialize(m_read_stream, reflect_type, value)) {
            if (TypeBase* type_base  = reflect_type.get_reflected_type_or_null()) {
                archive_fields(type_base, value);
            }
        }
    } else if (has_flags(AF_Out)) {
        ZENO_CHECK_MSG(m_write_stream && m_write_stream->is_valid(), "Write stream is invalid")
        if (!m_primary_serializer->serialize(m_write_stream, reflect_type, value)) {
            if (TypeBase* type_base  = reflect_type.get_reflected_type_or_null()) {
                archive_fields(type_base, value);
            }
        }
    }

    return *this;
}

Archive& Archive::archive_any(Any& serialize_object) {
    ZENO_CHECK_MSG(m_primary_serializer, "Primary serializer shouldn't be empty");

    if (has_flags(AF_In)) {
        ZENO_CHECK_MSG(m_read_stream && m_read_stream->is_valid(), "Read stream is invalid")
        if (!m_primary_serializer->deserialize(m_read_stream, serialize_object)) {
            try_recursive_archive(serialize_object);
        }
    } else if (has_flags(AF_Out)) {
        ZENO_CHECK_MSG(m_write_stream && m_write_stream->is_valid(), "Write stream is invalid")
        if (!m_primary_serializer->serialize(m_write_stream, serialize_object)) {
            try_recursive_archive(serialize_object);
        }
    }

    return *this;
}


bool Archive::try_recursive_archive(Any& serialize_object) {
    Any _result = Any::make_null();
    Any self = to_any();

    if (TypeBase* type_data = TypeHandle(serialize_object.type()).get_reflected_type_or_null()) {
        if (SerializeFunctionDelegate(serialize_object, _result, ArrayList<Any*> { &self } )) {
            return true;
        }

        // Check auto serialize metadata
        const IRawMetadata* metadata = type_data->get_metadata();
        if (metadata && metadata->get_value("AutoSerialize") != nullptr) {
            archive_fields(type_data, serialize_object);
            return true;
        }
    }

    return false;
}
