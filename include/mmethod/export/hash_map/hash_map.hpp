//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_HASH_MAP_BASE_HPP
#define BOOST_MMETHOD_HASH_MAP_BASE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include "boost/mmethod/export/hash_map/bucket.hpp"

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

// All functions declared in this file
// are defined in rtti/mmethod/hash_map.cpp

namespace boost {
namespace mmethod {
namespace hash_detail {

class hash_map
{
public:
  typedef bucket_t const* iterator;

private:
  /// hash function, thread-safe
  index_type hash(key_type a) const BOOST_NOEXCEPT_OR_NOTHROW;

public:
#ifndef BOOST_NO_CXX11_SMART_PTR
  BOOST_CONSTEXPR
#endif
  inline hash_map() BOOST_NOEXCEPT_OR_NOTHROW;

  // delayed creation
  void create(std::size_t sz);

private:
  // delete copy/assign
  BOOST_DELETED_FUNCTION(hash_map(hash_map const&));
  BOOST_DELETED_FUNCTION(hash_map& operator=(hash_map const&));

  // provide custom move operation
  void move(hash_map&) BOOST_NOEXCEPT_OR_NOTHROW;

public:
  /// fallback value
  //@{
  value_type fallback() const { return m_fallback; }
  void   set_fallback(value_type v) { m_fallback = v; }
  //@}

public:
  /// find block -> all these are thread-safe
  //@{
  iterator find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;

private:
  iterator do_find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;
  //@}

public:
  /// insert block -> all these must be protected by mutex
  //@{
  void insert(key_type key, value_type value);

private:
  void insert_need_resize(key_type key, value_type value);
  //@}

private:
#ifdef BOOST_NO_CXX11_SMART_PTR
  typedef boost::scoped_array<bucket_t> array_type;
#else
  typedef std::unique_ptr<bucket_t[]> array_type;
#endif

  std::size_t m_mask; // == (1 << m_logsz) - 1
  array_type  m_array;

  std::size_t m_logsz;

  value_type  m_fallback;
};

}}} // namespace boost::mmethod::hash_detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
