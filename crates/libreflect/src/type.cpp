#include "reflect/type.hpp"

zeno::reflect::TypeHandle::TypeHandle(TypeBase *type_info)
    : m_handle(type_info)
{
}

zeno::reflect::TypeBase::TypeBase(const ReflectedTypeInfo &type_info)
 : m_type_info(type_info) {}
