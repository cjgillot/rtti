//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"
#include "product.hpp"

#include <list>

#include <boost/range/begin.hpp>
#include <boost/optional.hpp>
#include <boost/bind.hpp>

/* Implementation of pole computation algorithm from [2]
 * 
 * Algorithms are described in terms of this paper.
 * 
 * [1] Pirkelbauer, Solodkyy, Stroustrup. Report on language support
 * for Multi-Methods and Open-Methods for C++.
 */

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch,
  ambiguity_handler_t ahndl
);

void boost_mmethod_dispatch::dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
) {
  // the order given by product_incr is
  // a topological order for signature_t::worse_match,
  // each base is dispatched before any of its derived
  for(product_t p (pole_table);
      p.valid();
      p.incr()
  ) {
    signature_t sig (p.deref());
    dispatch_one(sig, pole_table, dispatch, ahndl);
  }
}

typedef std::list<overload_t> max_set_type;

static void insert_upcasts(signature_t const& sig0, dispatch_t const& dispatch, max_set_type& max_set);
static void filter_insert(max_set_type& max_set, overload_t const& up);

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch,
  ambiguity_handler_t ahndl
) {
  // already registered
  if(dispatch.count(sig))
    return;

  // set of candidates
  max_set_type max_set;
  insert_upcasts(sig, dispatch, max_set);

  if(max_set.size() == 1)
    dispatch.insert(std::make_pair( sig, max_set.front() ));

  else {
    dispatch.insert(std::make_pair( sig, overload_t(sig, NULL) ));

    if(ahndl) {
      std::size_t const arity = sig.array().size();

      std::vector<rtti_type> amb; amb.resize(arity);

      for(std::size_t k = 0; k < arity; ++k)
        amb[k] = sig.array()[k]->get_id();

      ahndl(arity, amb.data());
    }
  }
}

static void insert_upcasts(
  signature_t const& sig0,
  dispatch_t const& dispatch,
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
      overload_t const& bound = dispatch.at(sig);

      // call filter
      filter_insert(max_set, bound);
    }
  }
}

// poll [max_set] and insert if good match
static void filter_insert(
  max_set_type& max_set
, overload_t const& up
) {
  signature_t::worse_match worse;

  max_set_type::iterator
    iter = max_set.begin()
  , endl = max_set.end();

  while(iter != endl) {
    overload_t const& e = *iter;

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
