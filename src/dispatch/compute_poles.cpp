//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "foreach.hpp"

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <deque>

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

//!\brief Compute rank and subtypes bitset
void
hierarchy_t::pole_init(klass_t* k) {
  std::size_t r = current_rank++;
  k->set_rank(r);
}

//!\brief Pseudo-closest algorithm (Fig 9)
static std::size_t
pseudo_closest(klass_t const* klass, const klass_t* *out_pole) {
  BOOST_ASSERT(out_pole);

  // compute candidates
  std::vector<klass_t const*> const& candidates = klass->get_bases();

  // trivial cases
  if(candidates.empty())
    return 0;

  if(candidates.size() == 1) {
    *out_pole = candidates.front();
    return 1;
  }

  // compare to maximal element
  klass_t const* const maxK = *std::max_element(
    candidates.begin(), candidates.end(),
    rank_compare()
  );

  foreach(klass_t const* k, candidates)
    // degenerate case
    if( !klass_t::is_subtype_of()(*maxK, *k) )
      return 2;

  // assign and return
  *out_pole = maxK;
  return 1;
}

/// topological sort
//@{

namespace {

typedef std::vector<rtti_hierarchy> input_type;

//!\brief Topological sort traversal functor
//! klass objects are popped from the most general
//! to the most derived type
struct wanderer_t {
  std::deque<rtti_hierarchy> stack;
  boost::unordered_map<rtti_hierarchy, bool> visited;

  explicit wanderer_t(std::size_t) {}

  typedef rtti_hierarchy value_type;
  typedef value_type const& const_reference;

  bool empty() const { return stack.empty(); }

  void push_back(rtti_hierarchy);
  rtti_hierarchy pop();

private:
  bool reinject_bases(rtti_hierarchy);
};

// is_pole is used as a traversal flag
void wanderer_t::push_back(rtti_hierarchy k) {
  visited[k] = false;
  stack.push_back(k);
}

rtti_hierarchy wanderer_t::pop() {
  for(;;) {
    // exit condition
    if(stack.empty())
      return NULL;

    // get next element
    rtti_hierarchy top = stack.back();
    stack.pop_back();

    // already traversed ?
    if(visited[top])
      continue;

    // inject base classes
    bool const need_upcast = reinject_bases(top);

    // retry if a base has been injected
    if(need_upcast) {
      stack.push_front(top);
      continue;
    }

    // mark as traversed
    visited[top] = true;
    foreach_base(rtti_hierarchy b, top)
      BOOST_ASSERT(visited[b]);

    return top;
  }
}

bool wanderer_t::reinject_bases(rtti_hierarchy top_pole) {
  bool need_upcast = false;

  foreach_base(rtti_hierarchy next, top_pole) {
    // not visited yet
    if(!visited[next]) {
      stack.push_back(next);
      need_upcast = true;
    }
  }

  return need_upcast;
}

} // namespace <>

//@}

void hierarchy_t::compute_poles(input_type const& input) {
  // primary poles
  foreach(rtti_hierarchy hh, input)
    this->add(hh);

  // prepare traversal structure
  wanderer_t wanderer ( input.size() );
  std::copy(
    input.begin(), input.end(),
    std::back_inserter(wanderer)
  );

  // traverse
  while(rtti_hierarchy top = wanderer.pop()) {
    poles_map_t::const_iterator pit = poles.find(top);

    if( pit != poles.end() ) {
      // primary pole case
      klass_t* k = pit->second;

      BOOST_ASSERT( std::find(input.begin(), input.end(), k->get_rtti()) != input.end() );

      this->pole_init(k);
    }
    else {
      // non-primary pole
      klass_t* k = this->add(top);

      // compute
      klass_t const* pole;
      std::size_t const sz = pseudo_closest(k, &pole);

      if(sz <= 1) {
        // false pole
        this->remove(k);
      }
      else {
        // effective pole found
        this->pole_init(k);
      }
    }
  }

  // sort
  std::sort(klasses.begin(), klasses.end(), rank_compare());
}
