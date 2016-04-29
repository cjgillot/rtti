//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "product.hpp"

#include <boost/range/adaptor/transformed.hpp>

product_t::product_t(const pole_table_t &pt)
: exhausted(false)
, table(pt)
, range(pt.size())
{
  for(std::size_t k = 0; k < range.size(); ++k) {
    hier_t const& r = table[k].range();

    range[k] = boost::begin(r);
    if(range[k] == boost::end(r)) {
      exhausted = true;
    }
  }
}

void product_t::incr()
{
  for(std::size_t k = 0; k < range.size(); ++k)
  {
    hier_t const& r = table[k].range();

    ++range[k];
    if( range[k] != boost::end(r) ) {
      return;
    }

    range[k] = boost::begin(r);
  }

  exhausted = true;
}
