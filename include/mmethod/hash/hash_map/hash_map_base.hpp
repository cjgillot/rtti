#ifndef RTTI_MMETHOD_HASH_MAP_BASE_HPP
#define RTTI_MMETHOD_HASH_MAP_BASE_HPP

#include "rtti/rttifwd.hpp"
#include "rtti/detail/attribute.hpp"

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include "mmethod/hash/hash_map/bucket.hpp"

// All functions declared in this file
// are defined in rtti/mmethod/hash_map_base.cpp

namespace rtti {
namespace hash {
namespace detail {

class hash_map;
class hash_map_base
: private boost::noncopyable
{
public:
  typedef bucket_t const* iterator;

  friend class hash_map;

public:
  inline hash_map_base();
  ~hash_map_base();

private:
  hash_map_base(hash_map_base const&);
  hash_map_base& operator=(hash_map_base const&);
#ifdef BOOST_HAS_RVALUE_REFS
  hash_map_base(hash_map_base&&);
  hash_map_base& operator=(hash_map_base&&);
#endif

public:
  /// hash function, thread-safe
  index_type ATTRIBUTE_PURE hash(key_type a) const BOOST_NOEXCEPT_OR_NOTHROW;

public:
  /// find block -> all these are thread-safe
  //@{
  iterator ATTRIBUTE_PURE zero() const BOOST_NOEXCEPT_OR_NOTHROW;
  iterator ATTRIBUTE_PURE find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;

private:
  iterator ATTRIBUTE_PURE do_find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;
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
  void move(hash_map_base&) BOOST_NOEXCEPT_OR_NOTHROW;
  //@}

private:
  std::size_t m_mask; // == (1 << m_logsz) - 1
  
#ifndef BOOST_HAS_RVALUE_REFS
  boost::scoped_array<bucket_t> m_array;
#else
  std::unique_ptr<bucket_t[]> m_array;
#endif

  std::size_t m_logsz;
};

}}} // namespace rtti::hash::detail

#include "mmethod/hash/hash_map/hash_map_base.ipp"

#endif
