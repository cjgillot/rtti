#include "rtti/hash/hash_map.hpp"

#include "util/assert.hpp"

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::hash::detail::bucket_t;
using rtti::hash::detail::hash_map;

/// hash_map implementation
//@{
hash_map::~hash_map() {}

void hash_map::flush(hash_map const& m) {
  m_base.flush(m.m_base);
}

void hash_map::insert_at(iterator it, key_type key, value_type value) {
  return m_base.insert_at(it, key, value);
}

void hash_map::insert(key_type key, value_type value) { m_base.insert(key, value); }
void hash_map::erase(iterator it)                     { m_base.erase(it); }
//@}
