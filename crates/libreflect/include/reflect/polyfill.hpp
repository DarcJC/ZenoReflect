#pragma once

// Introduce feature testing header in C++20 and above
#if __cplusplus >= 202002L
#   include <version>
#endif

#define LIBREFLECT_INLINE inline

#define REFLECT_FORCE_CONSTEPXR constexpr

#if __cpp_constexpr >= 201907L
#define REFLECT_CONSTEXPR constexpr
#define REFLECT_CONSTEXPR_OR_INLINE constexpr
#define REFLECT_CONSTEXPR_OR_CONST constexpr
#else
#define REFLECT_CONSTEXPR 
#define REFLECT_CONSTEXPR_OR_INLINE inline
#define REFLECT_CONSTEXPR_OR_CONST const
#endif

#if __cpp_constexpr >= 202211L
#define REFLECT_STATIC_CONSTEXPR constexpr
#else
#define REFLECT_STATIC_CONSTEXPR
#endif

#if __cpp_concepts >= 202002L
#define REFLECT_SUPPORT_CONCEPTS 1
#else
#define REFLECT_SUPPORT_CONCEPTS 0
#endif

#if defined (__cpp_lib_execution)
#   if __cpp_lib_execution >= 201902L
#       define REFLECT_EXECUTION_CAN_VECTORIZATION std::execution::unseq
#   else
#       define REFLECT_EXECUTION_CAN_VECTORIZATION std::execution::seq
#   endif // __cpp_lib_execution >= 201902L
#else // !defined (__cpp_lib_execution)
#   define REFLECT_EXECUTION_CAN_VECTORIZATION 
#endif // defined (__cpp_lib_execution)
