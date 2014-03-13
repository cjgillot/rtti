//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BASE_IPP
#define RTTI_MMETHOD_HASH_MAP_BASE_IPP

#include "mmethod/hash/hash_map/hash_map_base.hpp"
#include "mmethod/hash/hash_map/bucket.ipp"

namespace rtti {
namespace hash {
namespace detail {

inline hash_map_base::hash_map_base()
: m_mask(0), m_logsz(0) {}

}}} // namespace rtti::mmethod::detail

#include "mmethod/hash/hash_map/hash_map_base.tpp"

#endif
