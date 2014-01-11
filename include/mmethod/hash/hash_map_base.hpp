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
private:
  typedef std::uintptr_t storage_type;

  // assume even values
  key_type m_key;
  storage_type m_value;

public:
  constexpr bucket_t()
  : m_key(rtti_type(0ul)), m_value(1) {}
  constexpr bucket_t(key_type k, value_type v)
  : m_key(k), m_value( static_cast<storage_type>(v) ) {}
  
  inline key_type   key()   const { return m_key;   }
  inline value_type value() const { return static_cast<value_type>(m_value); }

  inline bool empty() const
  { return m_value & 1; }

  inline void set(key_type k, value_type v);
  inline void reset();
};

class hash_map;
class hash_map_base {
public:
  typedef bucket_t const* iterator;

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
  /// hash function, thread-safe
  inline index_type ATTRIBUTE_PURE hash(key_type a) const noexcept;

public:
  /// find block -> all these are thread-safe
  //@{
  iterator ATTRIBUTE_PURE zero() const noexcept;
  iterator ATTRIBUTE_PURE find(key_type key) const noexcept;

private:
  iterator ATTRIBUTE_PURE do_find(key_type key) const noexcept;
  //@}

public:
  /// insert block -> all these must be protected by mutex
  //@{
  void create(std::size_t logsz);
  void insert(key_type key, value_type value);
  void insert_at(iterator it, key_type key, value_type value);
  void erase(iterator it);

  void flush(hash_map_base const&);

private:
  void insert_need_resize(key_type key, value_type value);
  void move(hash_map_base&&) noexcept;
  //@}

private:
  std::size_t m_mask { 0 }; // == (1 << m_logsz) - 1
  std::unique_ptr<bucket_t[]> m_array;

  std::size_t m_logsz = 0;
};

}}} // namespace rtti::hash::detail

#endif
