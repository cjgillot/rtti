#ifndef RTTI_MMETHOD_HASH_MAP_BUCKET_HPP
#define RTTI_MMETHOD_HASH_MAP_BUCKET_HPP

#include "rtti/detail/attribute.hpp"
#include "rtti/rttifwd.hpp"

namespace rtti {
namespace hash {
namespace detail {

typedef rtti_type       key_type;
typedef uintptr_t  value_type;
typedef size_t     index_type;

struct bucket_t {
private:
  typedef uintptr_t storage_type;

  // assume even values
  key_type      m_key;
  storage_type  m_value;

public:
  BOOST_CONSTEXPR bucket_t() BOOST_NOEXCEPT_OR_NOTHROW;
  BOOST_CONSTEXPR bucket_t(key_type k, value_type v) BOOST_NOEXCEPT_OR_NOTHROW;

  // access
  key_type   ATTRIBUTE_PURE key()   const BOOST_NOEXCEPT_OR_NOTHROW;
  value_type ATTRIBUTE_PURE value() const BOOST_NOEXCEPT_OR_NOTHROW;
  bool       ATTRIBUTE_PURE empty() const BOOST_NOEXCEPT_OR_NOTHROW;

  // modification
  void set(key_type k, value_type v) BOOST_NOEXCEPT_OR_NOTHROW;
  void reset() BOOST_NOEXCEPT_OR_NOTHROW;
};

}}} // namespace rtti::hash::detail

#endif
