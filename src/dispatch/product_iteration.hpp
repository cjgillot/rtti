//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MMETHOD_PRODUCT_ITERATION_HPP
#define MMETHOD_PRODUCT_ITERATION_HPP

#include "hierarchy.hpp"

#include <boost/range/adaptor/transformed.hpp>

namespace product_iteration {

typedef hierarchy_t::range_type                    range_t;
typedef boost::range_const_iterator<range_t>::type iter_t;
typedef std::vector<iter_t>                        product_t;

struct beginner {
  iter_t operator()(hierarchy_t const& p) const
  { return p.range().begin(); }
};

void product_alloc(product_t& p, const pole_table_t &table)
{
  p.resize(table.size());
  std::transform(
    table.begin(), table.end(),
    p.begin(),
    beginner()
  );
}

bool product_incr(product_t& p, const pole_table_t &table)
{
  for(std::size_t k = 0; k < p.size(); ++k)
  {
    range_t const& r = table[k].range();

    ++p[k];
    if( p[k] != r.end() ) return true;
    p[k] = r.begin();
  }
  return false;
}

struct deref
: std::unary_function<iter_t, const klass_t*>
{
  result_type operator()(const argument_type& it) const
  { return *it; }
};

boost::transformed_range<deref, product_t>
product_deref(product_t& p)
{
  return boost::adaptors::transform(p, deref());
}

} // namespace product_iteration

using product_iteration::product_t;
using product_iteration::product_incr;
using product_iteration::product_deref;
using product_iteration::product_alloc;

#endif
