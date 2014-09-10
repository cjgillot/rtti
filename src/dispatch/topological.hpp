//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef TOPOLOGICAL_ITERATION_HPP
#define TOPOLOGICAL_ITERATION_HPP

#include "early.hpp"

#include <boost/unordered_set.hpp>
#include <deque>

//!\brief Topological sort traversal functor
//! klass objects are popped from the most general
//! to the most derived type
struct wanderer_t {
  explicit wanderer_t(std::size_t) {}

  typedef rtti_hierarchy value_type;
  typedef value_type const& const_reference;

  bool empty() const { return stack.empty(); }

  void push_back(rtti_hierarchy);
  rtti_hierarchy pop();

private:
  bool reinject_bases(rtti_hierarchy);

private:
  std::deque<rtti_hierarchy> stack;
  boost::unordered_set<rtti_hierarchy> visited;
};

#endif
