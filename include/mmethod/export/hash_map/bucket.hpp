//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_HASH_MAP_BUCKET_HPP
#define BOOST_MMETHOD_HASH_MAP_BUCKET_HPP

#include <stdint.h>

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace hash_detail {

typedef rtti_type    key_type;
typedef uintptr_t  value_type;
typedef  intptr_t  index_type;

struct bucket_t {
private:
  typedef uintptr_t storage_type;

  // assume even values
  storage_type  m_key;
  storage_type  m_value;

public:
  BOOST_CONSTEXPR bucket_t() BOOST_NOEXCEPT_OR_NOTHROW;

  // access
  key_type   key()   const BOOST_NOEXCEPT_OR_NOTHROW;
  value_type value() const BOOST_NOEXCEPT_OR_NOTHROW;
  bool       empty() const BOOST_NOEXCEPT_OR_NOTHROW;

  // modification
  void set(key_type k, value_type v) BOOST_NOEXCEPT_OR_NOTHROW;
  void reset() BOOST_NOEXCEPT_OR_NOTHROW;
};

}}} // namespace boost::mmethod::hash_detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
