#pragma once

#include <cstdint>
#include <type_traits>
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
    class IMemberField;

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

    enum class Qualifier : uint8_t {
        None = 0,
        Const = 1 << 0,
        Static = 1 << 1,
        Volatile = 1 << 2,
        NoExcept = 1 << 3,
        Mutable = 1 << 4,
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
        virtual const RTTITypeInfo& get_rtti_info() const = 0;
        virtual const ReflectedTypeInfo& get_info() const;

        virtual const ArrayList<ITypeConstructor*>& get_constructors() const = 0;
        virtual const ArrayList<IMemberFunction*>& get_member_functions() const = 0;
        virtual const ArrayList<IMemberField*>& get_member_fields() const = 0;
        virtual ArrayList<ITypeConstructor*> get_constructor(const ArrayList<RTTITypeInfo>&) const;
        virtual ITypeConstructor* get_constructor_or_null(const ArrayList<RTTITypeInfo>& params) const;
        virtual ITypeConstructor& get_constructor_checked(const ArrayList<RTTITypeInfo>& params) const;
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
            if REFLECT_FORCE_CONSTEPXR (VTIsPointer<T>) {
                return { type_info<std::decay_t<T>*>() };
            } else {
                return { type_info<std::decay_t<T>>() };
            }
        }
    
        inline static REFLECT_STATIC_CONSTEXPR TypeHandle nulltype() {
            return { T_NullTypeArg{} };
        }
    private:
        REFLECT_STATIC_CONSTEXPR TypeHandle(const T_NullTypeArg&);
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

    /// Reflection Interfaces
    class LIBREFLECT_API IHasParameter {
    public:
        virtual ~IHasParameter();

        virtual bool is_suitable_with_params(const ArrayList<RTTITypeInfo>& types = {}) const;
        virtual bool is_suitable_to_invoke(const ArrayList<Any>& params = {}) const;

        virtual const ArrayList<RTTITypeInfo>& get_params() const = 0;
        virtual const ArrayList<RTTITypeInfo>& get_params_dacayed() const = 0;

        virtual Any get_param_default_value(size_t index) = 0;
    };

    class LIBREFLECT_API IHasName {
    public:
        virtual ~IHasName();

        virtual StringView get_name() = 0;
    };

    class LIBREFLECT_API IBelongToParentType {
    public:
        virtual TypeHandle get_parent_type() const;
        virtual ~IBelongToParentType();
    protected:
        explicit IBelongToParentType(TypeHandle in_type);

        TypeHandle m_type;
    };

    class LIBREFLECT_API IHasQualifier {
    public:
        virtual ~IHasQualifier();

        virtual bool is_static() const;
        virtual bool is_const() const;
        virtual bool is_volatile() const;
        virtual bool is_no_except() const;
        virtual bool is_mutable() const;
    };

    class LIBREFLECT_API IHasMetadata {
    public:
        virtual ~IHasMetadata();
    };

    class LIBREFLECT_API ITypeConstructor : public IBelongToParentType, public IHasParameter {
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
    protected:
        explicit ITypeConstructor(TypeHandle in_type);
    };

    class LIBREFLECT_API IMemberFunction : public IBelongToParentType, public IHasParameter, public IHasName, public IHasQualifier, public IHasMetadata {
    public:
        virtual ~IMemberFunction();
        virtual TypeHandle get_return_type() const = 0;
        virtual Any invoke(const Any& clazz_object, const ArrayList<Any>& params = {}) const = 0;
        virtual Any invoke_static(const ArrayList<Any>& params = {}) const = 0;

    protected:
        explicit IMemberFunction(TypeHandle in_type);
    };

    class LIBREFLECT_API IMemberField : public IBelongToParentType, public IHasName, public IHasMetadata {
    public:
        template <typename T>
        T* get_field_ptr_typed(const Any& clazz_object) {
            if (get_field_type() != get_type<T>()) {
                return nullptr;
            }
            return static_cast<T*>(get_field_ptr(clazz_object));
        }

        virtual ~IMemberField();
        virtual void* get_field_ptr(const Any& clazz_object) const = 0;
        virtual TypeHandle get_field_type() const = 0;

    protected:
        explicit IMemberField(TypeHandle in_type);
    };

}
}
