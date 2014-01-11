#ifndef RTTI_MMETHOD_HASH_MAP_IPP
#define RTTI_MMETHOD_HASH_MAP_IPP

#include "mmethod/hash/hash_map/hash_map.hpp"
#include "mmethod/hash/hash_map/hash_map_base.ipp"

namespace rtti {
namespace hash {
namespace detail {

inline constexpr hash_map::hash_map() {}

namespace {
inline constexpr std::size_t
nbits(std::size_t n) {
  return (n == 0)
    ? 0
    : 1 + nbits(n >> 1)
    ;
}
} // namespace <>

template<std::size_t N>
inline void hash_map::create() {
  constexpr std::size_t logsz = 1 + nbits(N | 7);
  m_base.create(logsz);
}

inline hash_map::iterator ATTRIBUTE_PURE hash_map::zero()             const noexcept { return m_base.zero();    }
inline hash_map::iterator ATTRIBUTE_PURE hash_map::find(key_type key) const noexcept { return m_base.find(key); }

}}} // namespace rtti::mmethod::detail

#ifndef MMETHOD_IN_MMETHOD

#include "mmethod/hash/hash_map/hash_map_base.tpp"
#include "mmethod/hash/fetch_pole.hpp"

#endif

#endif
