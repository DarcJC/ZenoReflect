#include "reflect/type.hpp"
#include "reflect/registry.hpp"

using namespace zeno::reflect;

TypeHandle::TypeHandle(TypeBase *type_info)
    : m_handle(type_info)
    , is_reflected_type(true)
{
}


bool TypeHandle::operator==(const TypeHandle& other) const {
    TypeBase* self_type = nullptr;
    if (is_reflected_type) {
        self_type = m_handle.type_info;
    } else {
        self_type = get_reflected_type_or_null();
    }
}

bool TypeHandle::operator!=(const TypeHandle& other) const {
    return !(other == *this);
}

TypeBase* TypeHandle::operator->() const
{
    return get_reflected_type_or_null();
}

TypeBase* TypeHandle::get_reflected_type_or_null() const
{
    if (is_reflected_type) {
        return m_handle.type_info;
    }
    TypeBase* reflected_type = ReflectionRegistry::get()->get(m_handle.rtti_hash);
    if (nullptr != reflected_type) {
        m_handle.type_info = reflected_type;
        is_reflected_type = true;
    }
    return reflected_type;
}

TypeBase::TypeBase(const ReflectedTypeInfo &type_info)
    : m_type_info(type_info)
{
}

bool TypeBase::operator==(const TypeBase &other) const
{
    return type_hash() == other.type_hash();
}

bool zeno::reflect::TypeBase::operator!=(const TypeBase &other) const
{
    return !(other == *this);
}
