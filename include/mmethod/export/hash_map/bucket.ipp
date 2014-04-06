//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_HASH_MAP_BUCKET_IPP
#define BOOST_MMETHOD_HASH_MAP_BUCKET_IPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/export/hash_map/bucket.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace hash_detail {

BOOST_CONSTEXPR inline
bucket_t::bucket_t() BOOST_NOEXCEPT_OR_NOTHROW
: m_key( rtti_type(0ul) ), m_value(1)
{}

BOOST_CONSTEXPR inline
bucket_t::bucket_t(key_type k, value_type v) BOOST_NOEXCEPT_OR_NOTHROW
: m_key(k), m_value( static_cast<storage_type>(v) )
{}

inline key_type   bucket_t::key()   const BOOST_NOEXCEPT_OR_NOTHROW { return m_key;   }

inline bool       bucket_t::empty() const BOOST_NOEXCEPT_OR_NOTHROW { return m_value & 1; }
inline value_type bucket_t::value() const BOOST_NOEXCEPT_OR_NOTHROW { return static_cast<value_type>(m_value); }

}}} // namespace boost::mmethod::hash_detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
