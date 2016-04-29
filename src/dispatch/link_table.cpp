//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "link_table.hpp"

#include "foreach.hpp"
#include "product.hpp"

link_table::link_table(dispatch_t& d, const pole_table_t& pt)
: dispatch(d), pole_table(pt)
{
  // Initial population
  foreach(overload_t const& p, dispatch) {
    insert_overload(p.first, p);
  }
}

void link_table::insert(const signature_t& s, link_t const& l) {
  links.insert(std::make_pair(s, l));
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

struct link_table::resolve_visitor
: public boost::static_visitor<invoker_t>
{
  explicit
  resolve_visitor(link_table* t)
  : self(t) {}

  invoker_t operator()(const signature_t& l2) const {
    return self->resolve_once(l2);
  }

  invoker_t operator()(const overload_t& ov) const {
    return ov.second;
  }

  invoker_t operator()(boost::none_t /*unused*/) const {
    return NULL;
  }

private:
  link_table* self;
};

// Need to care about loops
invoker_t link_table::resolve_once(const signature_t& lnk) {
  dispatch_t::iterator it = dispatch.find(lnk);
  if(it != dispatch.end()) {
    return it->second;
  }

  if(visited.find(lnk) != visited.end()) {
    dispatch.insert(std::make_pair(lnk, invoker_t(NULL)));
    return NULL;
  }

  visited.insert(lnk);

  BOOST_ASSERT(links.find(lnk) != links.end());
  link_t const& ol = links.at(lnk);

  invoker_t ret = boost::apply_visitor(resolve_visitor(this), ol);

  dispatch.insert(std::make_pair(lnk, ret));
  return ret;
}

