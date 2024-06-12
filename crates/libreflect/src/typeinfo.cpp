#include "reflect/typeinfo.hpp"
#include "typeinfo.hpp"
#include "container/string"

const char *zeno::reflect::RTTITypeInfo::name() const
{
    return m_name;
}

size_t zeno::reflect::RTTITypeInfo::hash_code() const
{
    return m_hashcode;
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
