#include "reflect/type.hpp"

zeno::reflect::TypeHandle::TypeHandle(TypeBase *type_info)
    : m_handle(type_info)
{
}