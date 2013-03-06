#ifndef RTTI_MMETHOD_POLES_HPP
#define RTTI_MMETHOD_POLES_HPP

#include "rtti/shared/basic.hpp"
#include "rtti/hash/hash_map.hpp"

namespace rtti {
namespace mmethod {
namespace detail {

using hash::detail::pole_t;
using hash::detail::poles_map_type;

typedef invoker_t const* const invoker_table_type;

}}} // namespace rtti::mmethod::detail

#endif
