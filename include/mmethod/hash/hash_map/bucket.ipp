#ifndef RTTI_MMETHOD_HASH_MAP_BUCKET_IPP
#define RTTI_MMETHOD_HASH_MAP_BUCKET_IPP

#include "bucket.hpp"

namespace rtti {
namespace hash {
namespace detail {

constexpr bucket_t::bucket_t() noexcept:                         bucket_t( rtti_type(0ul), 1 )                     {}
constexpr bucket_t::bucket_t(key_type k, value_type v) noexcept: m_key(k), m_value( static_cast<storage_type>(v) ) {}

inline key_type   bucket_t::key()   const noexcept { return m_key;   }

inline bool       bucket_t::empty() const noexcept { return m_value & 1; }
inline value_type bucket_t::value() const noexcept { return static_cast<value_type>(m_value); }

}}} // namespace rtti::hash::detail

#endif
