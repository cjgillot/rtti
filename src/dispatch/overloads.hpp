#ifndef RTTI_MPH_OVERLOADS_HPP
#define RTTI_MPH_OVERLOADS_HPP

#include <vector>

#include <algorithm>
#include <functional>

#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>

#include "hierarchy.hpp"
#include "signature.hpp"

#include "early.hpp"

typedef std::pair<signature_t, invoker_t> overload_t;

inline bool operator<(overload_t const& a, overload_t const& b)
{ return signature_t::total_order()(a.first, b.first); }

typedef std::vector<overload_t> overloads_t;

typedef std::map<signature_t, boost::optional<overload_t>, signature_t::total_order> dispatch_t;

namespace std {
  template<>
  struct hash<signature_t>
  {
    std::size_t operator()(const signature_t& s) const
    {
      return boost::hash_range(s.array().begin(), s.array().end());
    };
  };
}

#endif
