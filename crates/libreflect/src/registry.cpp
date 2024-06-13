#include "reflect/registry.hpp"
#include <map>
#include <atomic>
#include <limits>
#include <cstdlib>
#include <iostream>
#include "registry.hpp"

using namespace zeno::reflect;

int32_t internal_utils::allocate_new_id()
{
    static std::atomic<int32_t> GLOBAL_ID_COUNTER = 0;
    return GLOBAL_ID_COUNTER.fetch_add(1, std::memory_order_seq_cst);
}

ReflectionRegistry& ReflectionRegistry::get()
{
    static ReflectionRegistry instance{};
    return instance;
}

ReflectTypeMap* zeno::reflect::ReflectionRegistry::operator->()
{
    return &m_typed_map;
}

#define RTM_TO_TYPED_MAPS(var) static_cast < std::map < KeyType, ValueType> *>(var)

                                                                     zeno::reflect::ReflectTypeMap::ReflectTypeMap()
{
    m_opaque_data = new std::map<KeyType, ValueType>();
}

zeno::reflect::ReflectTypeMap::~ReflectTypeMap()
{
    auto* ptr = RTM_TO_TYPED_MAPS(m_opaque_data);
    delete ptr;
}

bool zeno::reflect::ReflectTypeMap::add(ValueType val)
{
    auto* ptr = RTM_TO_TYPED_MAPS(m_opaque_data);
    if (ptr->contains(val->type_hash())) {
        return false;
    }
    ptr->insert_or_assign(val->type_hash(), val);
    return true;
}

size_t zeno::reflect::ReflectTypeMap::size() const
{
    return RTM_TO_TYPED_MAPS(m_opaque_data)->size();
}

auto zeno::reflect::ReflectTypeMap::get(KeyType hash) -> ValueType
{
    auto* ptr = RTM_TO_TYPED_MAPS(m_opaque_data);
    if (auto it = ptr->find(hash); it != ptr->end()) {
        return it->second;
    }
    return nullptr;
}
