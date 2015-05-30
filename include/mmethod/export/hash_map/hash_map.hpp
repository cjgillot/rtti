//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_BASE_HPP
#define RTTI_MMETHOD_HASH_MAP_BASE_HPP

#include "mmethod/config.hpp"
#include "mmethod/rttifwd.hpp"

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include "mmethod/export/hash_map/bucket.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

// All functions declared in this file
// are defined in rtti/mmethod/hash_map.cpp

namespace rtti {
namespace hash {
namespace detail {

class hash_map
: private boost::noncopyable
{
public:
  typedef bucket_t const* iterator;

public:
  inline hash_map();

private:
  /// hash function, thread-safe
  index_type hash(key_type a) const BOOST_NOEXCEPT_OR_NOTHROW;

public:
  /// access table size -> thread-safe
  std::size_t size() const BOOST_NOEXCEPT_OR_NOTHROW;

public:
  /// find block -> all these are thread-safe
  //@{
  iterator zero() const BOOST_NOEXCEPT_OR_NOTHROW;
  iterator find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;

private:
  iterator do_find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;
  //@}

public:
  /// insert block -> all these must be protected by mutex
  //@{
  void create(std::size_t sz);
  void insert(key_type key, value_type value);
  void insert_at(iterator it, key_type key, value_type value);
  void erase(iterator it);

private:
  void insert_need_resize(key_type key, value_type value);
  void flush(hash_map const&);
  void move(hash_map&) BOOST_NOEXCEPT_OR_NOTHROW;
  //@}

private:
  std::size_t m_mask; // == (1 << m_logsz) - 1
  boost::scoped_array<bucket_t> m_array;

  std::size_t m_logsz;
};

}}} // namespace rtti::hash::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
