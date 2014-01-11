#ifndef RTTI_MMETHOD_HASH_MAP_BUCKET_HPP
#define RTTI_MMETHOD_HASH_MAP_BUCKET_HPP

#include "util/attribute.hpp"
#include "rtti/rttifwd.hpp"

namespace rtti {
namespace hash {
namespace detail {

typedef rtti_type       key_type;
typedef std::uintptr_t  value_type;
typedef std::size_t     index_type;

struct bucket_t {
private:
  typedef std::uintptr_t storage_type;

  // assume even values
  key_type      m_key;
  storage_type  m_value;

public:
  constexpr bucket_t() noexcept;
  constexpr bucket_t(key_type k, value_type v) noexcept;

  // access
  key_type   ATTRIBUTE_PURE key()   const noexcept;
  value_type ATTRIBUTE_PURE value() const noexcept;
  bool       ATTRIBUTE_PURE empty() const noexcept;

  // modification
  void set(key_type k, value_type v) noexcept;
  void reset() noexcept;
};

}}} // namespace rtti::hash::detail

#endif
