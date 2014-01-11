#ifndef RTTI_MMETHOD_POLES_HPP
#define RTTI_MMETHOD_POLES_HPP

#include "mmethod/shared/basic.hpp"
#include "mmethod/hash/hash_map/hash_map.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

using hash::detail::poles_map_type;

typedef invoker_t const* const invoker_table_type;

// used in generated code
using hash::detail::pole_t;

}}} // namespace rtti::mmethod::detail

#endif