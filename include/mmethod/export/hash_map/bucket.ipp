//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BUCKET_IPP
#define RTTI_MMETHOD_HASH_MAP_BUCKET_IPP

#include "mmethod/config.hpp"
#include "mmethod/export/hash_map/bucket.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace hash {
namespace detail {

BOOST_CONSTEXPR inline
bucket_t::bucket_t() BOOST_NOEXCEPT_OR_NOTHROW
: m_key(1ul), m_value(0ul)
{}

inline key_type   bucket_t::key()   const BOOST_NOEXCEPT_OR_NOTHROW { return reinterpret_cast<key_type>(m_key);   }

inline bool       bucket_t::empty() const BOOST_NOEXCEPT_OR_NOTHROW { return m_key & 1; }
inline value_type bucket_t::value() const BOOST_NOEXCEPT_OR_NOTHROW { return reinterpret_cast<value_type>(m_value); }

}}} // namespace rtti::hash::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
