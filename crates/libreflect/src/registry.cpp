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

RttiHashMap* zeno::reflect::ReflectionRegistry::getRttiMap()
{
    return &m_rtti_map;
}

#define RTM_TO_RTTI_MAPS(var) static_cast < std::map < size_t, RTTITypeInfo> *>(var)

zeno::reflect::RttiHashMap::RttiHashMap() {
    m_opaque_data = new std::map<size_t, RTTITypeInfo>();
}

zeno::reflect::RttiHashMap::~RttiHashMap() {
    auto* ptr = RTM_TO_RTTI_MAPS(m_opaque_data);
    delete ptr;
}

bool zeno::reflect::RttiHashMap::add(size_t code, const RTTITypeInfo& rtti) {
    auto* ptr = RTM_TO_RTTI_MAPS(m_opaque_data);
    if (ptr->find(code) != ptr->end()) {
        return false;
    }
    ptr->insert_or_assign(code, rtti);
    return true;
}

size_t zeno::reflect::RttiHashMap::size() const {
    return RTM_TO_RTTI_MAPS(m_opaque_data)->size();
}

RTTITypeInfo zeno::reflect::RttiHashMap::get(size_t hash) {
    auto* ptr = RTM_TO_RTTI_MAPS(m_opaque_data);
    if (auto it = ptr->find(hash); it != ptr->end()) {
        return it->second;
    }
    static REFLECT_STATIC_CONSTEXPR RTTITypeInfo Default = { "<default_type>", 0, 0 };
    return Default;
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
    if (ptr->find(val->type_hash()) != ptr->end()) {
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

auto zeno::reflect::ReflectTypeMap::all() const -> ArrayList<ValueType>
{
    ArrayList<ValueType> res(size());
    auto* ptr = RTM_TO_TYPED_MAPS(m_opaque_data);
    for (auto& [key, val] : *ptr) {
        res.add_item(val);
    }
    return res;
}

auto zeno::reflect::ReflectTypeMap::find_by_canonical_name(const StringView& in_view) -> ValueType 
{
    auto* ptr = RTM_TO_TYPED_MAPS(m_opaque_data);
    for (auto& [key, val] : *ptr) {
        if (val->get_info().canonical_typename == in_view) {
            return val;
        }
    }

    return nullptr;
}
