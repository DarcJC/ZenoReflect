#include "reflect/typeinfo.hpp"

const char *zeno::reflect::RTTITypeInfo::name() const
{
    return m_name.c_str();
}

size_t zeno::reflect::RTTITypeInfo::hash_code() const
{
    return m_hashcode;
}
