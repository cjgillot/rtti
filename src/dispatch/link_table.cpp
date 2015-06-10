//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "link_table.hpp"

#include "foreach.hpp"
#include "product.hpp"

typedef std::pair<signature_t, invoker_t> overload_t;

link_table::link_table(dispatch_t& d, const pole_table_t& pt)
: dispatch(d), pole_table(pt)
{
  // Initial population
  foreach(overload_t const& p, dispatch)
    links.insert(std::make_pair(
      p.first, link_t(p.first, variant_t(p.second))
    ));
}

void link_table::resolve() {
  for(product_t p (pole_table);
      p.valid();
      p.incr()
  ) {
    signature_t sig (p.deref());
    resolve_once(sig);
  }
}

// Need to care about loops
invoker_t link_table::resolve_once(const signature_t& lnk) {
  dispatch_t::iterator it = dispatch.find(lnk);
  if(it != dispatch.end())
    return it->second;

  if(visited.find(lnk) != visited.end()) {
    dispatch.insert(std::make_pair(lnk, invoker_t(NULL)));
    return NULL;
  }

  visited.insert(lnk);

  BOOST_ASSERT(links.find(lnk) != links.end());
  optional_link_t const& ol = links.at(lnk);

  invoker_t ret = NULL;
  if(ol) {
    variant_t const& var = ol->second;

    if(signature_t const* l2 = boost::get<signature_t>(&var)) {
      ret = resolve_once(*l2);
    }
    else {
      ret = boost::get<invoker_t>(var);
    }

    BOOST_ASSERT(ret);
  }

  dispatch.insert(std::make_pair(lnk, ret));
  return ret;
}

