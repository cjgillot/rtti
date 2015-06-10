//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"
#include "link_table.hpp"

#include "foreach.hpp"
#include "product.hpp"

#include <boost/variant.hpp>
#include <boost/unordered_set.hpp>
#include <boost/local_function.hpp>

/* Implementation of pole computation algorithm from [2]
 *
 * Algorithms are described in terms of this paper.
 *
 * [1] Pirkelbauer, Solodkyy, Stroustrup. Report on language support
 * for Multi-Methods and Open-Methods for C++.
 *
 * We add support for dynamic rebinding in case of ambiguity
 * by supporting "symbolic links" between overloads.
 */

typedef std::pair<signature_t, invoker_t> overload_t;

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  link_table& dispatch,
  ambiguity_handler_t ahndl
);

void rtti_dispatch::dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
) {
  link_table links (dispatch, pole_table);

  // the order given by product_incr is
  // a topological order for signature_t::worse_match,
  // each base is dispatched before any of its derived
  for(product_t p (pole_table);
      p.valid();
      p.incr()
  ) {
    signature_t sig (p.deref());
    dispatch_one(pole_table, sig, links, ahndl);
  }

  links.resolve();
}

typedef link_table::link_t link_t;
typedef std::list<link_t> max_set_type;

static void insert_upcasts(
  signature_t const& sig0,
  link_table const& dispatch,
  max_set_type& max_set
);
static void filter_insert(
  max_set_type& max_set,
  link_t const& up
);

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  link_table& dispatch,
  ambiguity_handler_t ahndl
) {
  // already registered
  if(dispatch.has(sig))
    return;

  // set of candidates
  max_set_type max_set;
  insert_upcasts(sig, dispatch, max_set);

  if(max_set.size() == 1) {
    dispatch.insert(sig, max_set.front());
  }

  else {
    BOOST_ASSERT(ahndl);

    std::size_t const arity = sig.array().size();

    std::vector<rtti_type> amb; amb.resize(arity);

    for(std::size_t k = 0; k < arity; ++k)
      amb[k] = sig.array()[k]->get_id();

    bool rebind = ahndl(arity, amb.data());
    if(rebind) {
      signature_t newsig ( arity );

      // generate classes
      for(size_t k = 0; k < arity; ++k)
        newsig.array_ref()[k] = pole_table[k].fetch(
          rtti::rtti_get_node(amb[k])
        );

      BOOST_ASSERT(newsig != sig);

      dispatch.insert_link(sig, newsig);
    }
    else {
      dispatch.insert_none(sig);
    }
  }
}

static void insert_upcasts(
  signature_t const& sig0,
  link_table const& dispatch,
  max_set_type& max_set
) {
  std::size_t const arity = sig0.array().size();

  for(std::size_t k = 0; k < arity; ++k) {
    klass_t const* kth = sig0.array()[k];

    foreach(klass_t const* base, kth->get_bases()) {
      // copy signature and replace
      signature_t sig = sig0;
      sig.array_ref()[k] = base;

      // we can safely use [dispatch.at] since all the candidates have been dispatched already
      link_table::optional_link_t const& bound = dispatch.at(sig);

      // call filter
      if(bound)
        filter_insert(max_set, *bound);
    }
  }
}

// poll [max_set] and insert if good match
static void filter_insert(
  max_set_type& max_set
, link_t const& up
) {
  signature_t::worse_match worse;

  max_set_type::iterator
    iter = max_set.begin()
  , endl = max_set.end();

  while(iter != endl) {
    link_t const& e = *iter;

    // [up] is better match, remove [e]
    if( worse(e.first, up.first) ) {
      iter = max_set.erase(iter);
      continue;
    }

    // [e] is better match, don't insert [up]
    else if( worse(up.first, e.first) ) {
      return;
    }

    // poll next overload
    ++iter;
  }

  // none of [max_set] is better
  max_set.push_back(up);
}
