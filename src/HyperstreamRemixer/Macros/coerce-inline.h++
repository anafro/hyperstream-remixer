#pragma once

#if defined(_MSC_VER)
#define __REMIXER_COERCE_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define __REMIXER_COERCE_INLINE __attribute__((always_inline)) inline
#else
#define __REMIXER_COERCE_INLINE inline
#endif
