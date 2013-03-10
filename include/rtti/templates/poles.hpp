#ifndef RTTI_DMETHOD_POLES_HPP
#define RTTI_DMETHOD_POLES_HPP

#include "rtti/shared/basic.hpp"
#include "rtti/hash/hash_map.hpp"

#if MMETHOD_USE_THREAD
#include "util/stw_lock.hpp"
#endif

#include <unordered_map>

namespace rtti {
namespace dmethod {
namespace detail {

typedef void* functor_t;

struct poles_map_type {
  hash::detail::hash_map mem;
  hash::detail::hash_map poles;

  std::size_t smallint = 0;

#if MMETHOD_USE_THREAD
  util::stw_lock lock;
#endif
};

struct invoker_entry;
struct invoker_table_type {
  invoker_entry* entries;

  typedef std::unique_ptr<std::uintptr_t[]> key_type;
  typedef functor_t value_type;
  struct hasher {
    std::size_t* arity;
    std::size_t operator()(key_type const& a) const;
  };
  struct equal {
    std::size_t* arity;
    bool operator()(key_type const& a, key_type const& b) const;
  };

  std::size_t arity = 0;

  hasher hash; equal eq;

  std::unordered_map<key_type, value_type, hasher, equal> mem   { 10, { &arity }, { &arity }};
  std::unordered_map<key_type, value_type, hasher, equal> poles { 10, { &arity }, { &arity }};

#if MMETHOD_USE_THREAD
  util::stw_lock lock;
#endif
};

}}} // namespace rtti::mmethod::detail

#endif
