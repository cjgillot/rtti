#ifndef RTTI_MMETHOD_HASH_MAP_HPP
#define RTTI_MMETHOD_HASH_MAP_HPP

#include "rtti/rttifwd.hpp"

#include "util/attribute.hpp"

#include <memory>

#include "util/strong_typedef.hpp"

// #define MMETHOD_USE_SMALLARRAY
#define MMETHOD_USE_THREAD

#ifdef MMETHOD_USE_THREAD
#include <mutex>
#include <atomic>
#endif

// All functions declared in this file
// are defined in rtti/mmethod/hash_map[_base].cpp

namespace rtti {
namespace mmethod {
namespace hash_map_detail {

typedef void(*invoker_t)();
static_assert(sizeof(std::uintptr_t) == sizeof(invoker_t), "Error");

typedef rtti_type key_type;
typedef std::uintptr_t value_type;
UTIL_STRONG_TYPEDEF(std::size_t, index_type)

struct bucket_t;

#ifdef MMETHOD_USE_THREAD
typedef std::atomic<bucket_t*> store_array_type;
typedef std::atomic<std::size_t> store_key_type;
typedef std::atomic<std::uintptr_t> store_value_type;
#else
typedef bucket_t* store_array_type;
typedef std::size_t store_key_type;
typedef std::uintptr_t store_value_type;
#endif

struct bucket_t {
  // assume even values
  store_key_type key;
  store_value_type value;

  constexpr bucket_t()
  : key(0), value(1) {}
  constexpr bucket_t(std::size_t k, std::size_t v)
  : key(k), value(v) {}

  inline bool empty() const
  { return value & 1; }

  inline void set(key_type k, value_type v);
  inline void reset();
};

class hash_map_base {
public:
  typedef bucket_t* iterator;

public:
  inline constexpr hash_map_base();
  ~hash_map_base();

private:
  hash_map_base(hash_map_base const&) = delete;
  hash_map_base(hash_map_base&&) = delete;
  hash_map_base& operator=(hash_map_base const&) = delete;
  hash_map_base& operator=(hash_map_base&&) noexcept;

protected:
  iterator ATTRIBUTE_PURE find(key_type key) const; // thread-safe

  void create(std::size_t logsz); // unsafe
  void insert(key_type key, value_type value); // unsafe

protected:
  // use by do_fetch_pole
  inline index_type hash(key_type a) const; // thread-safe
  iterator ATTRIBUTE_PURE zero() const; // thread-safe
  void insert_at(iterator it, key_type key, value_type value); // unsafe

protected:
  // unsafe block
  iterator ATTRIBUTE_PURE do_find(index_type index, key_type key) const;
  void do_create(std::size_t lsz);
  void insert_need_resize(key_type key, value_type value);

protected:
  std::size_t m_mask { 0 }; // == (1 << m_logsz) - 1
  std::unique_ptr<bucket_t[]> m_array;

  std::size_t m_logsz = 0;
};

class hash_map
: private hash_map_base {
public:
  using hash_map_base::iterator;

public:
#ifdef USE_SMALLARRAY
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
  // use by fetch_pole
  iterator ATTRIBUTE_PURE find(key_type key) const; // thread-safe
  std::uintptr_t ATTRIBUTE_PURE do_fetch_pole(const rtti_node*, hash_map::iterator) noexcept; // thread-safe

  // use by generated code
  template<std::size_t N> void create(); // unsafe
  void insert(key_type key, value_type value) noexcept; // unsafe

private:
  // use by do_fetch_pole
  iterator ATTRIBUTE_PURE zero() const; // thread-safe
  void insert_at(iterator it, key_type key, value_type value); // unsafe

private:
#ifdef MMETHOD_USE_SMALLARRAY
  std::size_t const m_smallcount;
  bucket_t* const m_smallarray;
#endif

#ifdef MMETHOD_USE_THREAD
  std::mutex m_mutex;
#endif
};

typedef bucket_t pole_t;
typedef hash_map poles_map_type;

//! thread-safe
inline std::uintptr_t
ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
fetch_pole(
  poles_map_type& map
, const ::rtti::detail::rtti_node* rt
) noexcept;

} // namespace hash_map_detail

namespace detail {

using hash_map_detail::pole_t;
using hash_map_detail::invoker_t;
using hash_map_detail::fetch_pole;
using hash_map_detail::poles_map_type;

}}} // namespace rtti::mmethod::detail

#endif

#include "rtti/mmethod/hash_map.ipp"
