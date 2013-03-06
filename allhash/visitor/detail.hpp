#ifndef RTTI_VISITOR_DETAIL_HPP
#define RTTI_VISITOR_DETAIL_HPP

#include "rtti/rttifwd.hpp"
#include "util/attribute.hpp"

// All functions declared in this file
// are defined in rtti/visitor/detail.cpp

namespace rtti {
namespace visitor {
namespace detail {

//! \brief Node type for vtable
//! This structure uses type-erased invoker type
//! to avoid unnecessary template code duplication.
struct vtable_node {
  typedef void(*invoker_t)();

  rtti::rtti_type id;
  invoker_t invoker;
};

//! \brief Invoker retrieval function
extern vtable_node::invoker_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(3)
fetch_node(
  const vtable_node* const beg
, const std::size_t vsz
, const ::rtti::detail::rtti_node* rt
) throw();

//! \brief Vtable initialization procedure
extern void
ATTRIBUTE_NONNULL(1)
sort_nodes(
  vtable_node* vt
, const std::size_t vsz
) throw();

}}} // namespace rtti::visitor::detail

#endif
