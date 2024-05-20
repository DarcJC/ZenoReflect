#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include <memory>
#include <atomic>
#include "reflect/macro.hpp"

namespace zeno
{
namespace reflect 
{
    /**
     * @brief Indicate which operation can be done on this type
    */
    enum class ZenoTypeTrait : uint32_t {
        None = 0,
        // Indicate this type will be copied after assignment
        Copy = 1 << 0,
        // Indicate this type support clone operation
        Clone = 1 << 1,
        // Can be invoked
        Callable = 1 << 2,
    };

    struct ZType {
        ZENO_NONCOPYABLE(ZType);

        std::string name;
        int32_t id = -1;
        uint32_t traits_bitmask = 0;
        std::shared_ptr<ZType> super_type;
    };

    class ZField {
        ZENO_NONCOPYABLE(ZField);

        std::shared_ptr<ZType> internal_type_info;

    public:
        typedef ZField Super;
        typedef ZField ThisClass;
        typedef ZField BaseFieldClass;

        void* get_from_parent_instance(void* parent);
    };

    class ZProperty : public ZField {
        ZENO_NONCOPYABLE(ZProperty);
        ZENO_DECLARE_FIELD_API(ZProperty, ZField);
    };

    /**
     * @note This is a base class for all reflected types.
    */
    class ZStruct : public ZField {
        ZENO_NONCOPYABLE(ZStruct);
        ZENO_DECLARE_FIELD_API(ZStruct, ZField);

        // The parent struct
        std::weak_ptr<ZStruct> super_struct;
        // The first element in a linked list of all children
        std::shared_ptr<ZProperty> property_link;
        std::shared_ptr<ZProperty> destructor_link;

        // Memory required in bytes to store this type, not including alignment.
        int32_t properties_size;
        int32_t min_alignment;
    };

    class ClassRegistry final {
    public:
        static ClassRegistry get();
    };

    struct internal_utils {
        static int32_t allocate_new_id();
    };
}
}

