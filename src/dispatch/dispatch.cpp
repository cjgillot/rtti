//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "forward.hpp"

#include "signature.hpp"

#include "max_set.hpp"
#include "link_table.hpp"

#include "foreach.hpp"
#include "product.hpp"

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

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  link_table_t& dispatch,
  ambiguity_handler_t ahndl
);

void rtti_dispatch::dispatch(
  dispatch_t &dispatch,
  const pole_table_t &pole_table,
  ambiguity_handler_t ahndl
) {
  link_table_t links (dispatch, pole_table);

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

static void dispatch_one(
  const pole_table_t &pole_table,
  const signature_t& sig,
  link_table_t& dispatch,
  ambiguity_handler_t ahndl
) {
  // already registered
  if(dispatch.has(sig)) {
    return;
  }

  // set of candidates
  max_set mset (sig, dispatch);

  if(mset.size() == 1) {
    dispatch.insert_overload(sig, mset.get());
    return;
  }

  BOOST_ASSERT(ahndl);

  std::size_t const arity = sig.array().size();

  std::vector<rtti_hierarchy> amb; amb.resize(arity);

  for(std::size_t k = 0; k < arity; ++k) {
    klass_t const* klass = sig.array()[k];
    amb[k] = rtti_get_node(klass->get_id());
  }

  using rtti::mmethod::ambiguity::action_t;

  action_t const action = ahndl(arity, amb.data());
  switch(boost::native_value(action)) {
    case action_t::NOTHING:
      dispatch.insert_none(sig);
      return;

    case action_t::RETRY: {
      signature_t newsig ( arity );

      // generate classes
      for(size_t k = 0; k < arity; ++k) {
        newsig.array_ref()[k] = pole_table[k].fetch_from(amb[k]);
      }

      BOOST_ASSERT(newsig != sig);

      dispatch.insert_link(sig, newsig);
    }
  }
}
