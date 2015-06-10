//          Copyright Camille Gillot 2012 - 2015.
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
public:
  typedef link_table::link_t link_t;

  // We use a list because filter() needs to erase elements
  // while iterating. list is the only non-assiciative container
  // guaranteeing iterators upon erase.
  typedef std::list<link_t> max_set_t;

private:
  max_set_t set;

public:
  max_set();

  void insert(const signature_t& sig, const link_table& links);

public:
  size_t size() const
  { return set.size(); }

  link_t get() const {
    BOOST_ASSERT(size() == 1);
    return set.front();
  }

private:
  void filter(const link_t& up);
};

#endif
