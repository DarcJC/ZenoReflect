#pragma once

#include <string>

namespace zeno
{
namespace reflect
{
    class RTTITypeInfo;

    template <typename T>
    const RTTITypeInfo& type_info();

    class RTTITypeInfo {
    public:
        RTTITypeInfo& operator=(const RTTITypeInfo&&) = delete;

        const char* name() const;
        size_t hash_code() const;

    private:
        RTTITypeInfo(std::string in_name, std::size_t hashcode): m_name(in_name), m_hashcode(hashcode) {}

        std::string m_name;
        size_t m_hashcode;

        template <typename T>
        friend const RTTITypeInfo& type_info();
    };

    // SFINAE
    template <typename T>
    const RTTITypeInfo& type_info() {
#ifdef ZENO_REFLECT_PROCESSING
        static RTTITypeInfo Default{"<default_type>", 0};
        return Default;
#else
        static_assert(false, "The type_info of current type not implemented");
#endif
    }
}
}
