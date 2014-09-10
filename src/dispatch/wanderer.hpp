//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef WANDERER_HPP
#define WANDERER_HPP

#include "hierarchy.hpp"

#include "foreach.hpp"

#include <boost/unordered_map.hpp>
#include <deque>

namespace rtti_dispatch {

//!\brief Topological sort traversal functor
//! klass objects are popped from the most general
//! to the most derived type.
class wanderer_t {
private:
  std::deque<rtti_hierarchy> stack;
  boost::unordered_map<rtti_hierarchy, bool> visited;

#ifndef NDEBUG
  // sanity test only in debug mode
  bool processing;
#endif

public:
  typedef rtti_hierarchy value_type;
  typedef value_type const& const_reference;

public:
  wanderer_t();

  //!\brief Add a hierarchy to the queue.
  //! This function shall not be called once
  //! something has been popped.
  void push_back(rtti_hierarchy k);

  //!\brief Fetch next hierarchy.
  //! This returns in order base->derived
  //! the objects in the hierarchy.
  //!\return NULL if the queue if exhausted
  rtti_hierarchy pop();

private:
  bool reinject_bases(rtti_hierarchy top_pole);
};

} // namespace rtti_dispatch

#endif
