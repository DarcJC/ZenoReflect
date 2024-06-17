#pragma once

#include <cstdint>
#include "reflect/polyfill.hpp"
#include "reflect/container/string"
#include "reflect/container/arraylist"
#include "reflect/container/any"
#include "reflect/macro.hpp"
#include "reflect/traits/type_traits"
#include "reflect/container/arraylist"

namespace zeno
{
namespace reflect 
{
    class TypeBase;
    class IHasParameter;
    class ITypeConstructor;
    class IMemberFunction;

    enum class TypeFlags : uint8_t {
        IsClass = 0,
        IsTemplateInstance,
        IsEnumeration,
        IsArray,
        IsPointer,
        IsArithmetic,
        IsFunctionPointer,
        IsMemberFieldPointer,
        IsMemberFunctionPointer,
        Max,
    };

    struct ReflectedTypeInfo {
        // A prefix name. Used to avoid name conflicts between difference modules
        StringView prefix;
        StringView qualified_name;
        StringView canonical_typename;
        bool internal_flags[static_cast<size_t>(TypeFlags::Max)] = { false };
    };

    class LIBREFLECT_API TypeBase {
    protected:
        TypeBase(const ReflectedTypeInfo& type_info);

        ReflectedTypeInfo m_type_info;
    public:
        TypeBase() = delete;
        virtual ~TypeBase() = default;

        virtual bool operator==(const TypeBase& other) const;
        virtual bool operator!=(const TypeBase& other) const;

        virtual std::size_t type_hash() const = 0;
        virtual const ReflectedTypeInfo& get_info() const;

        virtual const ArrayList<ITypeConstructor*>& get_constructors() const = 0;
        virtual const ArrayList<IMemberFunction*>& get_member_functions() const = 0;
        virtual ArrayList<ITypeConstructor*> get_constructor(const ArrayList<RTTITypeInfo>&) const;
    };

    struct T_NullTypeArg {};

    class LIBREFLECT_API TypeHandle {
        mutable union {
            TypeBase* type_info = nullptr;
            size_t rtti_hash;
        } m_handle;
        mutable bool is_reflected_type;

    public:
        TypeHandle(TypeBase* type_info);
        REFLECT_STATIC_CONSTEXPR TypeHandle(const RTTITypeInfo& rtti_info);

        bool operator==(const TypeHandle& other) const;
        bool operator!=(const TypeHandle& other) const;

        TypeBase* get_reflected_type_or_null() const;
        TypeBase* operator->() const;

        size_t type_hash() const;

        template <typename T>
        static REFLECT_STATIC_CONSTEXPR TypeHandle from_rtti() {
            return { type_info<TTDecay<T>>() };
        }
    
        inline static REFLECT_STATIC_CONSTEXPR TypeHandle nulltype() {
            return { T_NullTypeArg{} };
        }
    private:
        REFLECT_STATIC_CONSTEXPR TypeHandle(const T_NullTypeArg&);
    };

    class LIBREFLECT_API IHasParameter {
    public:
        virtual ~IHasParameter();

        virtual bool is_suitable_with_params(const ArrayList<RTTITypeInfo>& types = {}) const;
        virtual bool is_suitable_to_invoke(const ArrayList<Any>& params = {}) const;

        virtual const ArrayList<RTTITypeInfo>& get_params() const = 0;
        virtual const ArrayList<RTTITypeInfo>& get_params_dacayed() const = 0;
    };

    class LIBREFLECT_API ITypeConstructor : public IHasParameter {
    public:
        virtual ~ITypeConstructor();

        template <typename T>
        T* new_instance_typed(const ArrayList<Any>& params = {}) const {
            return reinterpret_cast<T*>(new_instance(params));
        }

        template <typename T>
        T create_instance_typed(const ArrayList<Any>& params = {}) const {
            return any_cast<T>(create_instance(params));
        }

        virtual void* new_instance(const ArrayList<Any>& params = {}) const = 0;

        virtual Any create_instance(const ArrayList<Any>& params = {}) const = 0;

        virtual TypeHandle get_parent_type() const;

    protected:
        explicit ITypeConstructor(TypeHandle in_type);

        TypeHandle m_type;
    };

    class LIBREFLECT_API IMemberFunction : public IHasParameter {
    };

    /// Utilities for type reflection

    /**
     * Returns the type info of a given type name.
     * 
     * The real implementation is generated by the reflection generation tool and included in target's pch.
    */
    template <typename T>
    LIBREFLECT_INLINE TypeHandle get_type() {
        return TypeHandle::from_rtti<T>();
    }
}
}
