#include "reflect/typeinfo.hpp"
#include "container/string"
#include "typeinfo.hpp"

zeno::reflect::RTTITypeInfo::RTTITypeInfo(const RTTITypeInfo & other) {
    m_name = other.m_name;
    m_hashcode = other.m_hashcode;
    m_decayed_hash = other.m_decayed_hash;
    m_flags = other.m_flags;
}

zeno::reflect::RTTITypeInfo::RTTITypeInfo(RTTITypeInfo && other)
    : m_name(other.m_name)
    , m_hashcode(other.m_hashcode)
    , m_decayed_hash(other.m_decayed_hash)
    , m_flags(other.m_flags)
{
    // As the name is stored as constant var, we will not release it
    other.m_name = nullptr;
    other.m_hashcode = 0;
    other.m_decayed_hash = 0;
}

zeno::reflect::RTTITypeInfo &zeno::reflect::RTTITypeInfo::operator=(const zeno::reflect::RTTITypeInfo &other)
{
    m_name = other.m_name;
    m_hashcode = other.m_hashcode;
    m_decayed_hash = other.m_decayed_hash;
    m_flags = other.m_flags;
    return *this;
}

zeno::reflect::RTTITypeInfo &zeno::reflect::RTTITypeInfo::operator=(zeno::reflect::RTTITypeInfo  &&other)
{
    if (&other != this) {
        m_name = other.m_name;
        m_hashcode = other.m_hashcode;
        m_decayed_hash = other.m_decayed_hash;
        m_flags = other.m_flags;
        other.m_name = nullptr;
        other.m_hashcode = 0;
        other.m_decayed_hash = 0;
        other.m_flags = TF_None;
    }
    return *this;
}

const char *zeno::reflect::RTTITypeInfo::name() const
{
    return m_name;
}

size_t zeno::reflect::RTTITypeInfo::hash_code() const
{
    return m_hashcode;
}

size_t zeno::reflect::RTTITypeInfo::flags() const
{
    return m_flags;
}

bool zeno::reflect::RTTITypeInfo::has_flags(size_t in_flags) const
{
    return m_flags & in_flags;
}

const size_t zeno::reflect::RTTITypeInfo::get_decayed_hash() const
{
    return m_decayed_hash;
}

REFLECT_STATIC_CONSTEXPR bool zeno::reflect::RTTITypeInfo::equal_fast(const RTTITypeInfo &other) const
{
    return hash_code() == other.hash_code();
}

REFLECT_STATIC_CONSTEXPR bool zeno::reflect::RTTITypeInfo::equal_fast_unsafe(const RTTITypeInfo &other) const
{
    return this == &other;
}

bool zeno::reflect::RTTITypeInfo::operator==(const RTTITypeInfo &other) const
{
    return other.hash_code() == other.hash_code() && CStringUtil<char>::strcmp(other.name(), name()) == 0;
}

bool zeno::reflect::RTTITypeInfo::operator!=(const RTTITypeInfo &other) const
{
    return !operator==(other);
}
