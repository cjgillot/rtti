//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#include <stdint.h>
#include <cstddef>

#include "mmethod/config.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace detail {

//! function pointer generic type
typedef void(*invoker_t)();

//! \brief RTTI id type
typedef void const* rtti_type;

//! \brief RTTI node type
//! POD class holding a hierarchy bottom-up
struct rtti_node;

//! \brief Useful typedef
typedef rtti_node const* rtti_hierarchy;

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

// Defined in node.hpp
inline rtti_type        MMETHOD_ATTRIBUTE_PURE rtti_get_id(rtti_node const* n);
inline rtti_node const* MMETHOD_ATTRIBUTE_PURE rtti_get_base(rtti_node const* n, std::size_t k = 0);
inline std::size_t      MMETHOD_ATTRIBUTE_PURE rtti_get_base_arity(rtti_node const* n);

} // namespace detail

// FIXME We should not export this
using detail::invoker_t;

using detail::rtti_type;
using detail::rtti_node;
using detail::rtti_hierarchy;

using detail::rtti_get_id;
using detail::rtti_get_base;
using detail::rtti_get_base_arity;

} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
