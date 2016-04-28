//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "foreach.hpp"
#include "wanderer.hpp"

#include <boost/range/algorithm/max_element.hpp>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <deque>

using namespace rtti_dispatch;

/* Implementation of pole computation algorithm from [1]
 *
 * Algorithms are described in terms of this paper.
 *
 * [1] Dujardin, Amiel, Simon. Fast Algorithms for Compressed Multi-Method
 *     Dispatch Tables Generation.
 */

// utils
namespace {

// reverse subtyping order - small is base
struct rank_compare {
  bool operator()(klass_t const* a, klass_t const* b) const
  { return klass_t::total_order()(*b,*a); }
};

} // namespace

//!\brief Create klass_t instance, with rank and subtypes bitset
void
hierarchy_t::add_pole(rtti_hierarchy vec, klass_t::bases_type& bases) {
  BOOST_ASSERT(vec);
  BOOST_ASSERT(!poles.count(vec));

  // create object
  klasses.push_back( new klass_t( vec ) );
  klass_t* k = klasses.back();

  // fill base classes
  k->bases.swap(bases);

  // register created class
  poles.insert(std::make_pair(vec, k));

  // initialize rank
  std::size_t r = current_rank++;
  k->set_rank(r);
}

//!\brief Compute registered bases
void
hierarchy_t::effective_bases(rtti_hierarchy klass, klass_t::bases_type* bases) {
  std::size_t const arity = rtti_get_base_arity(klass);

  bases->reserve(arity);
  foreach_base(rtti_hierarchy base, klass) {
    if(klass_t const* bk = try_fetch(base)) {
      bases->push_back(bk);
    }
  }
}

//!\brief Pseudo-closest algorithm (Fig 9)
std::size_t
hierarchy_t::pseudo_closest(
  rtti_hierarchy klass
, klass_t::bases_type const& candidates
, const klass_t* *out_pole
) {
  BOOST_ASSERT(out_pole);
  BOOST_ASSERT(!poles.count(klass));
  (void)klass;

  // trivial cases
  if(candidates.empty()) {
    // set NULL to mark dead end
    *out_pole = NULL;
    return 0;
  }

  if(candidates.size() == 1) {
    *out_pole = candidates.front();
    return 1;
  }

  // compare to maximal element
  klass_t const* const maxK = *boost::max_element(
    candidates,
    rank_compare());

  klass_t::is_subtype_of sub_cmp;
  foreach(klass_t const* k, candidates) {
    // degenerate case
    if( !sub_cmp(*maxK, *k) ) {
      return 2;
    }
  }

  // assign and return
  *out_pole = maxK;
  return 1;
}

typedef std::vector<rtti_hierarchy> input_type;

void hierarchy_t::compute_poles(input_type const& input) {
  // primary poles
  boost::unordered_set<rtti_hierarchy> primary_poles (
    boost::begin(input), boost::end(input)
  );

  // prepare traversal structure
  wanderer_t wanderer;
  std::copy(
    input.begin(), input.end(),
    std::back_inserter(wanderer)
  );

  // traverse
  while(rtti_hierarchy top = wanderer.pop()) {
    // candidate base classes
    klass_t::bases_type bases;
    effective_bases(top, &bases);

    if(primary_poles.count(top)) {
      // primary pole case
      BOOST_ASSERT(std::find(input.begin(), input.end(), top) != input.end());

      add_pole(top, bases);
    }
    else {
      // non-primary pole

      // compute
      klass_t const* pole;
      std::size_t const sz = pseudo_closest(top, bases, &pole);

      if(sz == 0) {
        // we have nothing
      }
      else if(sz == 1) {
        // save the pole
        BOOST_ASSERT(pole);
        BOOST_ASSERT(pole->get_rtti() != top);
        poles.insert(std::make_pair(top, pole));
      }
      else if(sz > 1) {
        // effective pole found
        add_pole(top, bases);
      }
    }
  }

  // sort
  std::sort(klasses.begin(), klasses.end(), rank_compare());
}
