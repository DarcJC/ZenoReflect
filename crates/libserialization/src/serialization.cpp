#include "reflect/serialization.hpp"

using namespace zeno::reflect;

Archive::Archive(UniquePtr<ISerializer> serializer/* = nullptr*/, UniquePtr<IReadableStream> read_stream/* = nullptr*/, UniquePtr<IWritableStream> write_stream/* = nullptr*/)
    : m_serializer(std::move(serializer))
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
