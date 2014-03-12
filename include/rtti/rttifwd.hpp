#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#include <stdint.h>
#include <cstddef>

namespace rtti {
namespace detail {

//! \brief RTTI id type
// UTIL_STRONG_TYPEDEF(uintptr_t, rtti_type);
typedef void* rtti_type;

//! \brief RTTI node type
//! POD class holding a hierarchy bottom-up
struct rtti_node {
//   rtti_type __id;
  std::size_t __arity;
  const rtti_node* __base [ 1 ];
};

//! \brief Variadic RTTI node type
template<size_t N>
struct rtti_node_var {
//   rtti_type __id;
  std::size_t __arity;
  const rtti_node* __base [ N ];
};
template<>
struct rtti_node_var<0> {
//   rtti_type __id;
  std::size_t __arity;
};

//! \brief Useful typedef
typedef ::rtti::detail::rtti_node const* rtti_hierarchy;

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

inline rtti_type
rtti_get_id(rtti_node const* n)
{ return (void*)n; }

} // namespace detail

using detail::rtti_type;
using detail::rtti_node;
using detail::rtti_hierarchy;

} // namespace rtti

#endif
