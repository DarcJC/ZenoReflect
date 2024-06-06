#include "reflect/typeinfo.hpp"

const char *zeno::reflect::RTTITypeInfo::name() const
{
    return m_name;
}

size_t zeno::reflect::RTTITypeInfo::hash_code() const
{
    return m_hashcode;
}

bool zeno::reflect::RTTITypeInfo::operator==(const RTTITypeInfo &other)
{
    return other.hash_code() == other.hash_code() && other.name() == name();
}

bool zeno::reflect::RTTITypeInfo::operator!=(const RTTITypeInfo &other)
{
    return !operator==(other);
}
