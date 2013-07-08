#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#include <cstdint>

#include "util/strong_typedef.hpp"

// #define RTTI_GETTER \
//   ::rtti::detail::rtti_getter

namespace rtti {
namespace detail {

//! \brief RTTI id type
// UTIL_STRONG_TYPEDEF(std::uintptr_t, rtti_type);
typedef std::uintptr_t rtti_type;

//! \brief RTTI node type
//! POD class holding a hierarchy bottom-up
struct rtti_node {
  rtti_type id;
  const rtti_node* base;
};

//! \brief Useful typedef
typedef ::rtti::detail::rtti_node const* rtti_hierarchy;

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

} // namespace detail

using detail::rtti_type;
using detail::rtti_node;
using detail::rtti_hierarchy;

} // namespace rtti

#endif
