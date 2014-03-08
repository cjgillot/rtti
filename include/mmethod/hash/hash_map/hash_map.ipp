#ifndef RTTI_MMETHOD_HASH_MAP_IPP
#define RTTI_MMETHOD_HASH_MAP_IPP

#include "mmethod/hash/hash_map/hash_map.hpp"
#include "mmethod/hash/hash_map/hash_map_base.ipp"

namespace rtti {
namespace hash {
namespace detail {

namespace {
inline std::size_t
nbits(std::size_t n) {
  std::size_t ret = 0;
  for(; n; n>>=1)
    ++ret;
  return ret;
}
} // namespace <>

inline void hash_map::create(std::size_t N) {
  std::size_t logsz = 1 + nbits(N | 7);
  m_base.create(logsz);
}

inline hash_map::iterator ATTRIBUTE_PURE hash_map::zero()             const BOOST_NOEXCEPT_OR_NOTHROW { return m_base.zero();    }
inline hash_map::iterator ATTRIBUTE_PURE hash_map::find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW { return m_base.find(key); }

}}} // namespace rtti::mmethod::detail

#include "mmethod/hash/hash_map/hash_map_base.tpp"

#endif
