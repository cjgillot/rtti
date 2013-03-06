#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#include <cstdint>

#define RTTI_GETTER \
  ::rtti::detail::rtti_getter

namespace rtti {
namespace detail {

//! \brief RTTI id type
typedef std::uintptr_t rtti_type;

//! \brief RTTI node type
//! POD class holding a hierarchy bottom-up
struct rtti_node {
  rtti_type id;
  const rtti_node* const base;
};

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

template<class T>
struct get_holder;

} // namespace detail

using detail::rtti_type;
using detail::rtti_node;

} // namespace rtti

#endif
