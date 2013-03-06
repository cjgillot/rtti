#ifndef RTTI_MMETHOD_HASH_MAP_BASE_HPP
#define RTTI_MMETHOD_HASH_MAP_BASE_HPP

#include "rtti/rttifwd.hpp"
#include "util/attribute.hpp"

#include <memory>

// All functions declared in this file
// are defined in rtti/mmethod/hash_map_base.cpp

namespace rtti {
namespace hash {
namespace detail {

typedef rtti_type key_type;
typedef std::uintptr_t value_type;
typedef std::size_t index_type;

struct bucket_t {
  // assume even values
  key_type key;
  value_type value;

  constexpr bucket_t()
  : key(rtti_type(0)), value(1) {}
  constexpr bucket_t(std::size_t k, std::size_t v)
  : key(rtti_type(k)), value(v) {}

  inline bool empty() const
  { return value & 1; }

  inline void set(key_type k, value_type v);
};

class hash_map;
class hash_map_base {
public:
  typedef bucket_t* iterator;
  friend class hash_map;

public:
  inline constexpr hash_map_base();
  ~hash_map_base();

private:
  hash_map_base(hash_map_base const&) = delete;
  hash_map_base(hash_map_base&&) = delete;
  hash_map_base& operator=(hash_map_base const&) = delete;
  hash_map_base& operator=(hash_map_base&&) = delete;

public:
  void import(hash_map_base const&) noexcept;
  void move(hash_map_base&) noexcept;

protected:
  /// hash function
  inline index_type ATTRIBUTE_PURE hash(key_type a) const noexcept; // thread-safe

public:
  /// find block -> all these are thread-safe
  //@{
  inline iterator ATTRIBUTE_PURE zero() const noexcept;
  inline iterator ATTRIBUTE_PURE find(key_type key) const noexcept;

private:
  iterator ATTRIBUTE_PURE do_find(key_type key) const noexcept;
  //@}

public:
  /// insert block -> all these must be protected by mutex
  //@{
  void create(std::size_t logsz);
  void insert(key_type key, value_type value);
  void insert_at(iterator it, key_type key, value_type value); // unsafe

private:
  void insert_need_resize(key_type key, value_type value);
  //@}

private:
  std::size_t m_mask { 0 }; // == (1 << m_logsz) - 1
  std::unique_ptr<bucket_t[]> m_array;

  std::size_t m_logsz = 0;
};

}}} // namespace rtti::hash::detail

#endif
