#pragma once

#if defined(__clang__)

#define DLLEXPORT __attribute__ ((__visibility__ ("default")))
#define DLLIMPORT
#define STDCALL __attribute__((stdcall))
#define CDECL __attribute__((cdecl))
#define RESTRICT __restrict__
#define FORCE_INLINE inline
#define OFFSET_OF(X, Y) __builtin_offsetof(X, Y)

#elif defined(__GNUC__)

#define DLLEXPORT __attribute__ ((__visibility__ ("default")))
#define DLLIMPORT
#define STDCALL __attribute__((stdcall))
#define CDECL __attribute__((cdecl))
#define RESTRICT __restrict__
#define FORCE_INLINE inline
#define OFFSET_OF(X, Y) __builtin_offsetof(X, Y)

#elif defined(_MSC_VER)

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#define STDCALL __stdcall
#define CDECL __cdecl
#define RESTRICT __restrict
#define FORCE_INLINE __forceinline
#define OFFSET_OF(X, Y) offsetof(X, Y)
#undef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCSIG__

#pragma warning(disable: 4005)

#else

#pragma error "Unknown compiler."

#endif
