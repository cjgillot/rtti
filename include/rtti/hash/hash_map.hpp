#ifndef RTTI_MMETHOD_HASH_MAP_HPP
#define RTTI_MMETHOD_HASH_MAP_HPP

#include "rtti/rttifwd.hpp"

#define MMETHOD_USE_SMALLARRAY 0
#define MMETHOD_USE_THREAD 0
#define MMETHOD_USE_INLINE_FIND 1
#define MMETHOD_USE_INLINE_DO_FIND 1

#include "rtti/hash/hash_map_base.hpp"

#include "util/attribute.hpp"

#if MMETHOD_USE_THREAD
#include <mutex>
#include <atomic>
#include <thread>

#include "util/stw_lock.hpp"
#endif

// All functions declared in this file
// are defined in rtti/mmethod/hash_map.cpp

namespace rtti {
namespace hash {
namespace detail {

class hash_map {
public:
  typedef hash_map_base::iterator iterator;

public:
#if MMETHOD_USE_SMALLARRAY
  template<std::size_t N>
  inline constexpr hash_map(bucket_t(&)[N]);
#else
  inline constexpr hash_map();
#endif
  ~hash_map();

private:
  hash_map(hash_map const&) = delete;
  hash_map(hash_map&&) = delete;
  hash_map& operator=(hash_map const&) = delete;
  hash_map& operator=(hash_map&&) = delete;

public:
  void flush(hash_map const&) noexcept;

public:
  /// find(), thread-safe
  //@{
  inline std::uintptr_t ATTRIBUTE_PURE fetch_pole(const ::rtti::detail::rtti_node* rt) const noexcept;
  inline iterator ATTRIBUTE_PURE find(key_type key) const noexcept;

private:
  inline iterator ATTRIBUTE_PURE zero() const noexcept;
  std::uintptr_t  ATTRIBUTE_PURE do_fetch_pole(const rtti_node*, hash_map::iterator) const noexcept;
  //@}

public:
  /// use by generated code, thread-unsafe
  //@{
  template<std::size_t N> void create(); // unsafe
  void insert(key_type key, value_type value); // unsafe
  void insert_at(iterator it, key_type key, value_type value); // unsafe

  void erase(iterator it);
  //@}

private:
#if MMETHOD_USE_SMALLARRAY
  static_assert( false, "SmallArray not supported yet" );
  std::size_t const m_smallcount;
  bucket_t* const m_smallarray;
#endif

  hash_map_base m_base;

#if MMETHOD_USE_THREAD
  mutable util::stw_lock m_mutex;
#endif
};

typedef bucket_t pole_t;
typedef hash_map poles_map_type;

}}} // namespace rtti::hash::detail

#endif

#include "rtti/hash/hash_map.ipp"
