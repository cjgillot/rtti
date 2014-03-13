//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_HASH_MAP_HPP
#define RTTI_MMETHOD_HASH_MAP_HPP

#include "rtti/rttifwd.hpp"

#define MMETHOD_USE_INLINE_DO_FIND 1

#include "mmethod/hash/hash_map/hash_map_base.hpp"

#include "rtti/detail/attribute.hpp"

// All functions declared in this file
// are defined in rtti/mmethod/hash_map.cpp

namespace rtti {
namespace hash {
namespace detail {

/// exported function
value_type do_fetch_pole(hash_map const& map, rtti_hierarchy rt0, hash_map_base::iterator it0) BOOST_NOEXCEPT_OR_NOTHROW;
value_type ATTRIBUTE_PURE ATTRIBUTE_NONNULL(2) ATTRIBUTE_HOT()
fetch_pole(hash_map const& map, rtti_hierarchy rt) BOOST_NOEXCEPT_OR_NOTHROW;

/// state class
class hash_map {
public:
  typedef hash_map_base::iterator iterator;

public:
  inline  hash_map() {}
  inline ~hash_map() {}

private:
  hash_map(hash_map const&);
  hash_map& operator=(hash_map const&);
#ifdef BOOST_HAS_RVALUE_REFS
  hash_map(hash_map&&);
  hash_map& operator=(hash_map&&);
#endif

public:
  void flush(hash_map const&);
  
public:
  /// find(), thread-safe
  //@{
  iterator ATTRIBUTE_PURE find(key_type key) const BOOST_NOEXCEPT_OR_NOTHROW;
  iterator ATTRIBUTE_PURE zero() const BOOST_NOEXCEPT_OR_NOTHROW;
  //@}

private:
  /// fetch_pole(), thread-safe
  //@{
  friend value_type fetch_pole(hash_map const&, rtti_hierarchy rt) BOOST_NOEXCEPT_OR_NOTHROW;
  friend value_type do_fetch_pole(hash_map const& map, rtti_hierarchy rt0, hash_map::iterator it0) BOOST_NOEXCEPT_OR_NOTHROW;
  inline bucket_t* array() const BOOST_NOEXCEPT_OR_NOTHROW { return m_base.m_array.get(); }
  //@}

public:
  /// use by generated code, thread-unsafe
  //@{
  void create(std::size_t N); // unsafe
  void insert(key_type key, value_type value); // unsafe
  void insert_at(iterator it, key_type key, value_type value); // unsafe

  void erase(iterator it);
  //@}

private:
  hash_map_base m_base;
};

typedef bucket_t pole_t;
typedef hash_map poles_map_type;

}}} // namespace rtti::hash::detail

#endif

#include "hash_map.ipp"
