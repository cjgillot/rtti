//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "foreach.hpp"
#include "product_iteration.hpp"

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

void rtti_dispatch::dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
) {
  // prepare traversal
  product_t p;
  product_alloc(p, pole_table);

  // the order given by product_incr is
  // a topological order for signature_t::subtypes,
  // each base is dispatched before any of its derived
  do {
    signature_t sig ( product_deref(p) );
    dispatch_one(sig, pole_table, dispatch, ahndl);
  }
  while( product_incr(p, pole_table) );
}

namespace {

struct sig_upcaster {
  signature_t const& sig0;
  std::size_t const arity;
  dispatch_t  const& dispatch;
  
  std::size_t k;
  std::size_t b;
  
  sig_upcaster(signature_t const& s0, std::size_t a, dispatch_t const& d);
  overload_t const* operator()() BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace <>

typedef std::list<overload_t> max_set_type;

static void filter_insert(max_set_type& max_set, overload_t const* up);

static void dispatch_one(
  const signature_t& sig,
  const pole_table_t &pole_table,
  dispatch_t &dispatch,
  ambiguity_handler_t ahndl
) {
  // already registered
  if(dispatch.find(sig) != dispatch.end())
    return;

  std::size_t const arity = pole_table.size();

  sig_upcaster upcast ( sig, arity, dispatch );

  // set of candidates
  max_set_type max_set;

  for(;;) {
    // new candidate to be tested
    overload_t const* up = upcast();

    // end loop ?
    if(!up)
      break;

    // insert in max_set
    filter_insert(max_set, up);
  }

  if(max_set.size() == 1)
    dispatch.insert(std::make_pair( sig, max_set.front() ));

  else {
    dispatch.insert(std::make_pair( sig, overload_t(sig, NULL) ));

    if(ahndl) {
      std::vector<rtti_type> amb; amb.resize(arity);

      for(std::size_t k = 0; k < arity; ++k)
        amb[k] = sig.array()[k]->get_id();

      ahndl(arity, amb.data());
    }
  }
}

sig_upcaster::sig_upcaster(const signature_t& s0, std::size_t a, const dispatch_t& d)
: sig0(s0), arity(a), dispatch(d)
, k(0), b(0) {}

overload_t const*
sig_upcaster::operator()() BOOST_NOEXCEPT_OR_NOTHROW
{
  for(;;) {
    signature_t sig = sig0;

    // continue upcast on base [b] of argument [k]
    klass_t const* nk = NULL;
    while(!nk) {
      // proceed to next argument
      while( b == sig.array()[k]->get_bases().size() ) {
        b = 0; ++k;
        if(k == arity)
          return NULL;
      }

      // select
      nk = sig.array()[k]->get_bases()[b];

      // prepare for next base
      ++b;
    }

    sig.array_ref()[k] = nk;

    // we can safely use [dispatch.at] since all the candidates have been dispatched already
    overload_t const& bound = dispatch.at(sig);

    return &bound;
  }
}

static void filter_insert(
  max_set_type& max_set
, overload_t const* up
) {
  signature_t::subtypes subtypes;

  // poll max_set
  bool dominated = false;
  {
    max_set_type::iterator
      iter = max_set.begin()
    , endl = max_set.end();

    while(iter != endl) {
      overload_t const& e = *iter;

      // [*up] is better match, remove [it]
      if( subtypes(e.first, up->first) )
        iter = max_set.erase(iter);

      // [it] is better match, don't insert [s2]
      else if( subtypes(up->first, e.first) )
      { dominated = true; break; }

      // continue
      else
        ++iter;
    }
  }

  // none of [max_set] is better
  if( !dominated )
    max_set.push_back(*up);
}
