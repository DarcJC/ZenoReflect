#include "reflect/type.hpp"
#include "reflect/registry.hpp"
#include "type.hpp"

using namespace zeno::reflect;

TypeHandle::TypeHandle(TypeBase *type_info)
    : is_reflected_type(true)
{
    m_handle.type_info = type_info;
}

zeno::reflect::TypeHandle::TypeHandle(const RTTITypeInfo &rtti_info)
    : is_reflected_type(false)
{
    m_handle.rtti_hash = rtti_info.hash_code();
}

bool TypeHandle::operator==(const TypeHandle& other) const {
    TypeBase* self_type = nullptr;
    if (is_reflected_type) {
        self_type = m_handle.type_info;
    } else {
        self_type = get_reflected_type_or_null();
    }

    TypeBase* other_type = other.get_reflected_type_or_null();

    if (nullptr == other_type || nullptr == self_type) {
        return nullptr == other_type && nullptr == self_type;
    }
    
    return *self_type == *other_type;
}

bool TypeHandle::operator!=(const TypeHandle& other) const {
    return !(other == *this);
}

TypeBase* TypeHandle::operator->() const
{
    return get_reflected_type_or_null();
}

size_t zeno::reflect::TypeHandle::type_hash() const
{
    if (is_reflected_type) {
        if (nullptr != m_handle.type_info) {
            return m_handle.type_info->type_hash();
        }
    } else {
        return m_handle.rtti_hash;
    }
    return 0;
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


REFLECT_STATIC_CONSTEXPR TypeHandle::TypeHandle(const T_NullTypeArg&) {
    m_handle.rtti_hash = 0;
    is_reflected_type = false;
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

const ReflectedTypeInfo& zeno::reflect::TypeBase::get_info() const
{
    return m_type_info;
}

zeno::reflect::ITypeConstructor::~ITypeConstructor() = default;

TypeHandle zeno::reflect::ITypeConstructor::get_parent_type() const
{
    return m_type;
}

zeno::reflect::ITypeConstructor::ITypeConstructor(TypeHandle in_type)
    : m_type(in_type)
{
}

zeno::reflect::IHasParameter::~IHasParameter() = default;

bool zeno::reflect::IHasParameter::is_suitable_to_invoke(const ArrayList<Any>& params) const {
    const ArrayList<RTTITypeInfo>& signature_erased = get_params_dacayed();
    if (params.size() < signature_erased.size()) {
        return false;
    }

    for (int i = 0; i < signature_erased.size(); ++i) {
        if (params[i].type() != signature_erased[i]) {
            return false;
        }
    }

    return true;
}
