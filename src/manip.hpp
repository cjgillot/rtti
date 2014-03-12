#ifndef RTTI_MANIP_HPP
#define RTTI_MANIP_HPP

#include "rtti/rttifwd.hpp"

namespace rtti {

inline rtti_node const*
rtti_get_base(rtti_node const* n, std::size_t k = 0)
{ return n->__base[k]; }

inline std::size_t
rtti_get_base_arity(rtti_node const* n)
{ return n->__arity; }

}

#endif
