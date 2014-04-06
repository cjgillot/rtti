//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// allow room for some optimizations

#include "boost/mmethod/config.hpp"

#ifndef BOOST_MMETHOD_ATTRIBUTE_PURE
#if defined(__GNUC__)
#define BOOST_MMETHOD_ATTRIBUTE_PURE __attribute__((pure))
#else
#define BOOST_MMETHOD_ATTRIBUTE_PURE
#endif
#endif

#ifndef BOOST_MMETHOD_ATTRIBUTE_ALIGNED
#ifdef __GNUC__
#define BOOST_MMETHOD_ATTRIBUTE_ALIGNED __attribute__((aligned(2)))
#else
#define BOOST_MMETHOD_ATTRIBUTE_ALIGNED
#endif
#endif
