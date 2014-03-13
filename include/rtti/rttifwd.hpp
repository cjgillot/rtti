#ifndef RTTI_FWD_HPP
#define RTTI_FWD_HPP

#include <stdint.h>
#include <cstddef>

namespace rtti {
namespace detail {

//! \brief RTTI id type
typedef void* rtti_type;

//! \brief RTTI node type
//! POD class holding a hierarchy bottom-up
struct rtti_node;

//! \brief Useful typedef
typedef rtti_node const* rtti_hierarchy;

//! \brief Friend structure for in-class rtti access
struct rtti_getter;

inline rtti_type
rtti_get_id(rtti_node const* n)
{ return (void*)n; }

} // namespace detail

namespace adl {

void rtti_get_mixin(...);
void rtti_parents_size(...);
void rtti_parents_foreach(...);

} // namespace adl

using detail::rtti_type;
using detail::rtti_node;
using detail::rtti_hierarchy;

} // namespace rtti

#endif
