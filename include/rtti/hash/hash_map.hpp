#ifndef RTTI_MMETHOD_HASH_MAP_HPP
#define RTTI_MMETHOD_HASH_MAP_HPP

#include "rtti/rttifwd.hpp"

#define MMETHOD_USE_THREAD 0
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

/// exported function
value_type ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
fetch_pole(hash_map const& map, rtti_hierarchy rt) noexcept;

/// state class
class hash_map {
public:
  typedef hash_map_base::iterator iterator;

public:
  inline constexpr hash_map();
  ~hash_map();

private:
  hash_map(hash_map const&) = delete;
  hash_map(hash_map&&) = delete;
  hash_map& operator=(hash_map const&) = delete;
  hash_map& operator=(hash_map&&) = delete;

public:
  void flush(hash_map const&);
  
public:
  /// find(), thread-safe
  //@{
  iterator ATTRIBUTE_PURE find(key_type key) const noexcept;
  iterator ATTRIBUTE_PURE zero() const noexcept;
  //@}

private:
  /// fetch_pole()
  //@{
  friend value_type ATTRIBUTE_PURE fetch_pole(hash_map const&, rtti_hierarchy rt) noexcept;
  inline bucket_t* array() const noexcept { return m_base.m_array.get(); }
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
