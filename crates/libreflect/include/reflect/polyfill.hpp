#pragma once

#define LIBREFLECT_INLINE inline

#if __cpp_constexpr >= 201907L
#define REFLECT_CONSTEXPR constexpr
#else
#define REFLECT_CONSTEXPR LIBREFLECT_INLINE
#endif

