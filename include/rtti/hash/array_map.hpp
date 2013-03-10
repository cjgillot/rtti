#ifndef RTTI_MMETHOD_ARRAY_MAP_HPP
#define RTTI_MMETHOD_ARRAY_MAP_HPP

#error "Not usable in sync with hash_map"

#include <cstdint>

#include "rtti/rttifwd.hpp"
#include "util/attribute.hpp"

// All functions declared in this file
// are defined in rtti/mmethod/poles.cpp

namespace rtti {
namespace mmethod {
namespace detail {

typedef void(*invoker_t)();
static_assert(sizeof(std::uintptr_t) == sizeof(invoker_t), "Error");

//! \brief Pole information type
//! This structure stores an external object identifying
//! the referred type, typically its hash.
struct pole_t {
  rtti::rtti_type id;
  std::uintptr_t data;
};

typedef std::size_t poles_map_size;
typedef pole_t      poles_map_type[];

//! \brief Invoker retrieval function
//! \invariant This function asserts that there exists a
//!   sentinel at (beg+vsz), ie a field ensuring
//!   that <tt>rt->id < (beg+csz)->id</tt> for any \c rt
//! \invariant when called, this function assumes that <tt>beg[rt->id] != rt->id</tt>
extern std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(3)
do_fetch_pole(
  const pole_t* const beg
, const std::size_t vsz
, const ::rtti::detail::rtti_node* rt
) noexcept;

extern void
ATTRIBUTE_NONNULL(1)
sort_poles(
  pole_t* const beg
, const std::size_t vsz
) noexcept;

inline std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(3)
fetch_pole(
  poles_map_type beg
, poles_map_size vsz
, const ::rtti::detail::rtti_node* rt
) noexcept {
  rtti_type id = rt->id;
  if( id < vsz && beg[id].id == id )
    return beg[id].data;

  return do_fetch_pole(beg, vsz, rt);
}

}}} // namespace rtti::mmethod::detail

#endif
