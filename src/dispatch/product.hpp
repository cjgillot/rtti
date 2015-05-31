//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_PRODUCT_ITERATION_HPP
#define MMETHOD_PRODUCT_ITERATION_HPP

#include "hierarchy.hpp"

#include <boost/range/adaptor/transformed.hpp>

struct product_t {
private:
  typedef hierarchy_t::range_type
    hier_t;
  typedef boost::range_const_iterator<hier_t>::type
    iter_t;
  typedef std::vector<iter_t>
    range_t;

  struct deref_op
  : std::unary_function<iter_t, const klass_t*>
  {
    result_type operator()(const argument_type& it) const
    { return *it; }
  };
  typedef boost::transformed_range<deref_op, range_t const>
    deref_t;

public:
  explicit product_t(pole_table_t const&);
  void incr();

  bool valid() const { return !exhausted; }
  deref_t deref() const {
    return boost::adaptors::transform(range, deref_op());
  }

private:
  bool exhausted;
  pole_table_t const& table;
  std::vector<iter_t> range;
};

#endif
