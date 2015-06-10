//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

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

typedef boost::variant<invoker_t, signature_t> variant_t;
typedef boost::optional<variant_t> link_t;
typedef std::pair<signature_t, link_t> overload_link_t;
typedef boost::unordered_map<signature_t, overload_link_t> dispatch_link_t;

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  dispatch_link_t& dispatch,
  ambiguity_handler_t ahndl
);

static void resolve_links(
  const pole_table_t &pole_table,
  const dispatch_link_t& links,
  dispatch_t& dispatch
);

void rtti_dispatch::dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
) {
  dispatch_link_t links;
  foreach(overload_t const& p, dispatch)
    links.insert(std::make_pair(
      p.first, overload_link_t(p.first, variant_t(p.second))
    ));

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

  resolve_links(pole_table, links, dispatch);
}

typedef std::list<overload_link_t> max_set_type;

static void insert_upcasts(
  signature_t const& sig0,
  dispatch_link_t const& dispatch,
  max_set_type& max_set
);
static void filter_insert(
  max_set_type& max_set,
  overload_link_t const& up
);

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  dispatch_link_t& dispatch,
  ambiguity_handler_t ahndl
) {
  // already registered
  if(dispatch.count(sig))
    return;

  // set of candidates
  max_set_type max_set;
  insert_upcasts(sig, dispatch, max_set);

  if(max_set.size() == 1) {
    dispatch.insert(std::make_pair( sig, max_set.front() ));
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

      dispatch.insert(std::make_pair(
        sig, overload_link_t(sig, variant_t(newsig))
      ));
    }
    else {
      dispatch.insert(std::make_pair(
        sig, overload_link_t(sig, boost::none)
      ));
    }
  }
}

static void insert_upcasts(
  signature_t const& sig0,
  dispatch_link_t const& dispatch,
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
      overload_link_t const& bound = dispatch.at(sig);

      // call filter
      filter_insert(max_set, bound);
    }
  }
}

// poll [max_set] and insert if good match
static void filter_insert(
  max_set_type& max_set
, overload_link_t const& up
) {
  if(!up.second)
    return;

  signature_t::worse_match worse;

  max_set_type::iterator
    iter = max_set.begin()
  , endl = max_set.end();

  while(iter != endl) {
    overload_link_t const& e = *iter;
    BOOST_ASSERT(e.second);

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

typedef boost::unordered_set<signature_t> visited_t;

// Need to care about loops
static invoker_t resolve_one_link(
  const signature_t& lnk,
  const dispatch_link_t& links,
  visited_t& visited,
  dispatch_t& dispatch
) {
  dispatch_t::iterator it = dispatch.find(lnk);
  if(it != dispatch.end())
    return it->second;

  if(visited.find(lnk) != visited.end()) {
    dispatch.insert(std::make_pair(lnk, invoker_t(NULL)));
    return NULL;
  }

  visited.insert(lnk);

  BOOST_ASSERT(links.find(lnk) != links.end());
  overload_link_t const& ol = links.at(lnk);

  invoker_t ret = NULL;
  if(ol.second) {
    variant_t const& var = *ol.second;

    if(signature_t const* l2 = boost::get<signature_t>(&var)) {
      ret = resolve_one_link(*l2, links, visited, dispatch);
    }
    else {
      ret = boost::get<invoker_t>(var);
    }

    BOOST_ASSERT(ret);
  }

  dispatch.insert(std::make_pair(lnk, ret));
  return ret;
}

static void resolve_links(
  const pole_table_t& pole_table,
  const dispatch_link_t& links,
  dispatch_t& dispatch
) {
  visited_t visited;

  for(product_t p (pole_table);
      p.valid();
      p.incr()
  ) {
    signature_t sig (p.deref());
    resolve_one_link(sig, links, visited, dispatch);
  }
}
