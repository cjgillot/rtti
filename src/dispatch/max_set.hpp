//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MPH_MAX_SET_HPP
#define RTTI_MPH_MAX_SET_HPP

#include "forward.hpp"
#include "link_table.hpp"

#include <list>
#include <boost/noncopyable.hpp>

class max_set
: private boost::noncopyable
{
  // We use a list because filter() needs to erase elements
  // while iterating. list is the only non-assiciative container
  // guaranteeing iterators upon erase.
  typedef std::list<overload_t> max_set_t;

  max_set_t set;

public:
  //!\brief Create a \c max_set from a signature.
  //! This function looks for all the candidates
  //! for the resolution of \c sig.
  //! It recursively filters for non-best overloads,
  //! only leaving the maximal overload.
  max_set(const signature_t& sig, const link_table& links);

public:
  //!\brief Get the size of the \c max_set.
  //! If the size is one, there is exactly one
  //! candidate for resolution.
  size_t size() const
  { return set.size(); }

  //!\brief Get the candidate.
  overload_t get() const {
    BOOST_ASSERT(size() == 1);
    return set.front();
  }

private:
  void filter(const overload_t& up);
};

#endif
