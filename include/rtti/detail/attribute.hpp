//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// allow room for some optimizations

#ifndef ATTRIBUTE_PURE
#if defined(__GNUC__)
#define ATTRIBUTE_PURE __attribute__((pure))
#else
#define ATTRIBUTE_PURE
#endif
#endif

#ifndef ATTRIBUTE_CONST
#if defined(__GNUC__)
#define ATTRIBUTE_CONST __attribute__((const))
#else
#define ATTRIBUTE_CONST
#endif
#endif

#ifndef ATTRIBUTE_NONNULL
#if defined(__GNUC__)
#define ATTRIBUTE_NONNULL( arg ) __attribute__((nonnull( arg )))
#else
#define ATTRIBUTE_NONNULL( arg )
#endif
#endif

#ifndef ATTRIBUTE_NORETURN
#if defined(__GNUC__)
#define ATTRIBUTE_NORETURN __attribute__((noreturn))
#elif defined(MSC_VER)
#define ATTRIBUTE_NORETURN __declspec(noreturn)
#else
#define ATTRIBUTE_NORETURN
#endif
#endif

#ifndef ATTRIBUTE_DEPRECATED
#if defined(__GNUC__)
#define ATTRIBUTE_DEPRECATED __attribute__((deprecated))
#elif defined(MSC_VER)
#define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#else
#define ATTRIBUTE_DEPRECATED
#endif
#endif

// #ifndef ATTRIBUTE_DELETED
// #ifndef BOOST_NO_DELETED_FUNCTIONS
// #define ATTRIBUTE_DELETED = delete
// #else
// #define ATTRIBUTE_DELETED
// #endif
// #endif

#ifndef ATTRIBUTE_HOT
#ifdef __GNUC__
#define ATTRIBUTE_HOT() __attribute__((hot))
#else
#define ATTRIBUTE_HOT() /**/
#endif
#endif

#ifndef MMETHOD_ATTRIBUTE_ALIGNED
#ifdef __GNUC__
#define MMETHOD_ATTRIBUTE_ALIGNED __attribute__((aligned(2)))
#else
#define MMETHOD_ATTRIBUTE_ALIGNED
#endif
#endif

#ifndef LIKELY
#ifdef __GNUC__
#define   LIKELY(x) (bool(__builtin_expect(!!(x), 1)))
#define UNLIKELY(x) (bool(__builtin_expect(!!(x), 0)))
#else
#define   LIKELY(x) (!!(x))
#define UNLIKELY(x) (!!(x))
#endif
#endif
