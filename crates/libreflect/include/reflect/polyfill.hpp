#pragma once

#ifdef __cpp_constexpr
#define REFLECT_CONSTEXPR constexpr
#else
#define REFLECT_CONSTEXPR
#endif

#define LIBREFLECT_INLINE inline
