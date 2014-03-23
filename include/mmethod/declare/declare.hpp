//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_DECLARE_HPP
#define RTTI_MMETHOD_DECLARE_HPP

#include "mmethod/declare/register.hpp"
#include "mmethod/declare/helper.hpp"
#include "mmethod/declare/traits.hpp"

#define MMETHOD_TAG(name) BOOST_JOIN(rtti_mmethod_tags__, name)

#define DECLARE_MMETHOD(name, ret, sig) \
struct MMETHOD_TAG(name)                \
: rtti::mmethod::mmethod_register<      \
  MMETHOD_TAG(name)                     \
, ret sig   > {                         \
  template<typename> struct overload;   \
}; \
static MMETHOD_TAG(name) name /* ; */

#endif
