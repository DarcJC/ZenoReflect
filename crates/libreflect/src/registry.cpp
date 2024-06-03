#include "reflect/registry.hpp"

using namespace zeno::reflect;

int32_t zeno::reflect::internal_utils::allocate_new_id()
{
    static std::atomic<int32_t> GLOBAL_ID_COUNTER = 0;
    return GLOBAL_ID_COUNTER.fetch_add(1, std::memory_order_seq_cst);
}
