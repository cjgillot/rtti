//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_OVERLOADS_HPP
#define RTTI_MPH_OVERLOADS_HPP

#include <vector>

#include <algorithm>
#include <functional>

#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <boost/optional.hpp>

#include "hierarchy.hpp"
#include "signature.hpp"

#include "early.hpp"

typedef std::pair<signature_t, invoker_t> overload_t;

inline bool operator<(overload_t const& a, overload_t const& b)
{ return signature_t::total_order()(a.first, b.first); }

typedef std::vector<overload_t> overloads_t;

typedef boost::unordered_map<signature_t, boost::optional<overload_t> > dispatch_t;

inline std::size_t hash_value(const signature_t& s)
{
  return boost::hash_range(s.array().begin(), s.array().end());
}

#endif
