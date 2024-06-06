#pragma once

#define LIBREFLECT_INLINE inline

#if __cpp_constexpr >= 201907L
#define REFLECT_CONSTEXPR constexpr
#define REFLECT_CONSTEXPR_OR_INLINE constexpr
#else
#define REFLECT_CONSTEXPR 
#define REFLECT_CONSTEXPR_OR_INLINE inline
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
