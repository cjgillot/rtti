//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "foreach.hpp"
#include "wanderer.hpp"

#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>

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

//@{ Utils

namespace {

// reverse subtyping order - small is base
struct rank_compare {
  bool operator()(klass_t const* a, klass_t const* b) const
  { return klass_t::total_order()(*b,*a); }
};

} // namespace

//@}
//@{ add_pole

//!\brief Create klass_t instance,
//! with rank and subtypes bitset.
void
hierarchy_t::add_pole(rtti_hierarchy vec, klass_t::bases_type& bases) {
  BOOST_ASSERT(vec);
  BOOST_ASSERT(!poles.count(vec));

  //!Create object
  klasses.push_back( NULL );
  klass_t* k = klasses.back() = new klass_t( vec );

  //!Fill base classes
  k->bases.swap(bases);

  //!Register created class
  poles.insert(std::make_pair(vec, k));

  //!Initialize rank
  std::size_t r = current_rank++;
  k->set_rank(r);
}

//@}
//@{ effective_bases

//!\brief Compute registered bases
//! We only keep here the bases that are also poles.
static void
effective_bases(hierarchy_t const& hh,
                rtti_hierarchy klass,
                klass_t::bases_type* bases)
{
  std::size_t const arity = rtti_get_base_arity(klass);

  bases->reserve(arity);
  foreach_base(rtti_hierarchy base, klass) {
    klass_t const* bk = hh.try_fetch(base);

    if(bk) {
      bases->push_back(bk);
    }
  }
}

//@}
//@{ pseudo_closest

//!\brief Pseudo-closest algorithm (Fig 9)
static std::size_t
pseudo_closest(
  klass_t::bases_type const& candidates
, const klass_t* *out_pole
) {
  BOOST_ASSERT(out_pole);

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

//@}
//@{ compute_poles

typedef std::vector<rtti_hierarchy> input_type;

void hierarchy_t::compute_poles(input_type const& input) {
  //!Primary poles:
  // All the classes that appear in 'input'
  // appear as arguments of the mmethods.
  // Therefore, they all must be kept for
  // dispatch generation.
  boost::unordered_set<rtti_hierarchy> const primary_poles (
    boost::begin(input), boost::end(input)
  );

  //!Prepare traversal structure:
  // We iterate from primary poles
  // in topological order.
  // Any classes' bases must have
  // been processed before the class
  // itself is processed.
  wanderer_t wanderer;
  std::copy(
    input.begin(), input.end(),
    std::back_inserter(wanderer)
  );

  //!Traverse the tree in order.
  while(rtti_hierarchy top = wanderer.pop()) {
    // Candidate base classes
    klass_t::bases_type bases;
    effective_bases(*this, top, &bases);

    if(primary_poles.count(top)) {
      //!If we have a primary pole.
      // We just need to declare it as such.
      add_pole(top, bases);
    }
    else {
      //!We don't have primary pole.
      // We investigate bases' poles
      // to decide on its status.
      // There are three cases:
      // - no pole in bases, just drop.
      // - a single pole among basis,
      //   we can alias the class to the pole
      //   since it represents no new information.
      // - multiple poles appear among basis,
      //   mark as a secondary pole.

      klass_t const* pole = NULL;
      std::size_t const sz = pseudo_closest(bases, &pole);

      if(sz == 0) {
        // We have nothing,
        // drop it.
      }
      else if(sz == 1) {
        // Save the pole alias.
        BOOST_ASSERT(pole);
        BOOST_ASSERT(pole->get_rtti() != top);
        poles.insert(std::make_pair(top, pole));
      }
      else if(sz > 1) {
        // Secondary pole found.
        add_pole(top, bases);
      }
    }
  }

  //!Check the classes are sorted in rank order.
  // The ranks correspond to the visitation
  // order in topological sort.
  BOOST_ASSERT(boost::is_sorted(
    klasses, rank_compare()
  ));
}

//@}
