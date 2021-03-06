//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BUCKET_HPP
#define RTTI_MMETHOD_HASH_MAP_BUCKET_HPP

#include <stdint.h>

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace rtti {
namespace hash {
namespace detail {

typedef rtti_type    key_type;
typedef uintptr_t  value_type;
typedef  unsigned  index_type;

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

} // namespace detail
} // namespace hash
} // namespace rtti

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
