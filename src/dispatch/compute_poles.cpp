//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include <boost/foreach.hpp>
#include <deque>

#include "rtti/rtti.hpp"

/* Implementation of pole computation algorithm from [1]
 * 
 * Algorithms are described in terms of this paper.
 * 
 * [1] Dujardin, Amiel, Simon. Fast Algorithms for Compressed Multi-Method
 *     Dispatch Tables Generation.
 */

// utils
namespace {

struct rank_compare {
  bool operator()(klass_t const* a, klass_t const* b) const
  { return klass_t::total_order()(*b,*a); }
};

struct select_second {
  template<typename Pair>
  typename Pair::second_type
    operator()(Pair const& b) const
  { return b.second; }
};

} // namespace


//! \brief hierarchy compression
//! All non pole bases in \c seq are removed
//! \arg seq : vector of poles in the hierarchy
void
hierarchy_t::shrink(std::vector<klass_t const*>& seq) {
  BOOST_FOREACH(klass_t const* p0, seq) {
    BOOST_ASSERT(p0->pole == p0);

    klass_t* p = const_cast<klass_t*>(p0);
    BOOST_FOREACH(klass_t const*& b, p->bases)
      b = b->pole;

    std::sort(p->bases.begin(), p->bases.end());
    p->bases.erase( std::unique(p->bases.begin(), p->bases.end()), p->bases.end() );
    p->bases.erase( std::remove(p->bases.begin(), p->bases.end(), (klass_t const*)NULL), p->bases.end() );
  }
  
  std::sort(seq.begin(), seq.end(), rank_compare());
}

//!\brief Compute rank and subtypes bitset
void
hierarchy_t::pole_init(klass_t* k) {
  std::size_t r = current_rank++;
  k->rank = r;

  if(k->subtype.size() <= r)
    k->subtype.resize(1+r);

  BOOST_FOREACH(klass_t const* b, k->bases) {
    if(b->subtype.size() <= r)
      const_cast<klass_t*>(b)->subtype.resize(1+r);

    k->subtype |= b->subtype;
  }

  k->subtype.set(r);
}

//!\brief Pseudo-closest algorithm (Fig 9)
std::size_t
hierarchy_t::pseudo_closest(const klass_t* k, const klass_t*& pole)
{
  // compute candidates
  std::vector<klass_t const*> candidates;
  candidates.reserve(k->bases.size());

  BOOST_FOREACH(klass_t const* base, k->bases)
    if(base->pole)
      candidates.push_back(base->pole);

  if(candidates.size() == 0)
    return 0;

  if(candidates.size() == 1) {
    pole = candidates.front();
    return 1;
  }

  klass_t const* const maxK = *std::max_element(
    candidates.begin(), candidates.end(),
    rank_compare()
  );

  BOOST_FOREACH(klass_t const* k, candidates)
    if( !klass_t::subtypes()(*k, *maxK) )
      return 2;

  pole = maxK;
  return 1;
}

namespace {

//!\brief Topological sort traversal functor
//! klass objects are popped from the most general
//! to the most derived type
struct wanderer_t {
  std::deque<klass_t*> stack;

  wanderer_t(std::size_t) {}

  typedef klass_t* value_type;

  // is_pole is used as a traversal flag
  void push_back(klass_t const* k) {
    klass_t* next = const_cast<klass_t*>(k);
    next->is_pole() = false;
    stack.push_back(next);
  }

  klass_t* pop() {
    for(;;) {
      if(stack.empty())
        return NULL;

      // get next element
      klass_t* top = stack.back();
      stack.pop_back();

      // already traversed ?
      if(top->is_pole() )
        continue;

      // inject base classes
      bool need_upcast = false;
      BOOST_FOREACH(klass_t const* base, top->get_bases()) {
        klass_t* next = const_cast<klass_t*>(base);

        // not visited yet
        if(! next->is_pole() ) {
          stack.push_back(next);
          need_upcast = true;
        }
      }

      // retry if a base has been injected
      if(need_upcast) {
        stack.push_front(top);
        continue;
      }

      // mark as traversed
      top->is_pole() = true;

      return top;
    }
  }
  bool empty() const { return stack.empty(); }
};

} // namespace <>

void hierarchy_t::compute_poles(std::vector<klass_t const*>& seq) {
  // primary poles are marked by add()

  // Prepare room -> worst case
  BOOST_ASSERT( seq.empty() );
  seq.reserve(klasses.size());

#ifndef NDEBUG
  // assert structure
  BOOST_FOREACH(klass_t const* k, wanderer.stack)
    if(k->is_pole())
      BOOST_ASSERT( k->pole == k );
    else
      BOOST_ASSERT( !k->pole );
#endif

  // prepare traversal structure
  wanderer_t wanderer (dict.size());
  std::transform(
    dict.begin(), dict.end(),
    std::back_inserter(wanderer),
    select_second()
  );

  // traverse
  while(klass_t* top = wanderer.pop()) {
    BOOST_ASSERT( std::find(seq.begin(), seq.end(), top) == seq.end() );

    if(! top->pole) {

      // compute
      klass_t const* pole;
      std::size_t const sz = pseudo_closest(top, pole);

      if(sz == 0)
        top->pole = NULL;

      else if(sz == 1)
        top->pole = pole;

      else {
        top->pole = top;
        goto init_push;
      }
    }

    // init and push
    else {
    init_push:

      pole_init(top);
      top->pole = top;

      seq.push_back(top);
    }
  }
  
  shrink(seq);
}
