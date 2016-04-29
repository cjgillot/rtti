//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef WANDERER_HPP
#define WANDERER_HPP

#include "early.hpp"

#include <boost/unordered_set.hpp>
#include <deque>

namespace rtti_dispatch {

//!\brief Topological sort traversal object.
//! \c klass_t objects are popped from the most general
//! to the most derived type.
struct wanderer_t {
  wanderer_t();

  typedef rtti_hierarchy value_type;
  typedef value_type const& const_reference;

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

private:
  std::deque<rtti_hierarchy> stack;
  boost::unordered_set<rtti_hierarchy> visited;
};

} // namespace rtti_dispatch

#endif
