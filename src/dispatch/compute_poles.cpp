//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hierarchy.hpp"

#include "foreach.hpp"

#include <boost/unordered_map.hpp>
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
  foreach(klass_t const* pole0, seq) {
    BOOST_ASSERT(pole0->pole == pole0);

    klass_t* pole = const_cast<klass_t*>(pole0);

    // shortcut non-poles
    foreach(klass_t const*& base, pole->bases)
      base = base->pole;

    // cleanup bases array
    typedef klass_t::bases_type::iterator iterator_t;
    iterator_t // iterator pair representing valid slice of the array
      base_begin = pole->bases.begin()
    , base_end   = pole->bases.end();

    std::sort(base_begin, base_end);
    base_end = std::unique(base_begin, base_end);
    base_end = std::remove(base_begin, base_end, static_cast<klass_t const*>(NULL));
    pole->bases.erase(base_end, pole->bases.end());
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

  foreach(klass_t const* b, k->bases) {
    if(b->subtype.size() <= r)
      const_cast<klass_t*>(b)->subtype.resize(1+r);

    k->subtype |= b->subtype;
  }

  k->subtype.set(r);
}

//!\brief Pseudo-closest algorithm (Fig 9)
std::size_t
hierarchy_t::pseudo_closest(const klass_t* klass, const klass_t* *out_pole) {
  BOOST_ASSERT(pole);

  // compute candidates
  std::vector<klass_t const*> candidates;
  candidates.reserve(klass->bases.size());

  foreach(klass_t const* base, klass->bases)
    if(base->pole)
      candidates.push_back(base->pole);

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

namespace {

//!\brief Topological sort traversal functor
//! klass objects are popped from the most general
//! to the most derived type
struct wanderer_t {
  std::deque<klass_t*> stack;
  boost::unordered_map<klass_t const*, bool> visited;

  explicit
  wanderer_t(std::size_t) {}

  typedef klass_t* value_type;

  // is_pole is used as a traversal flag
  void push_back(klass_t const* k) {
    klass_t* next = const_cast<klass_t*>(k);
    visited[next] = false;
    stack.push_back(next);
  }

  klass_t* pop() {
    for(;;) {
      // exit condition
      if(stack.empty())
        return NULL;

      // get next element
      klass_t* top = stack.back();
      stack.pop_back();

      // already traversed ?
      if( visited[top] )
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

      return top;
    }
  }

  bool empty() const { return stack.empty(); }

private:
  bool reinject_bases(klass_t* top_pole) {
    bool need_upcast = false;

    foreach(klass_t const* base, top_pole->get_bases()) {
      klass_t* next = const_cast<klass_t*>(base);

      // not visited yet
      if(! visited[next] ) {
        stack.push_back(next);
        need_upcast = true;
      }
    }

    return need_upcast;
  }
};

} // namespace <>

void hierarchy_t::compute_poles(std::vector<klass_t const*>& seq) {
  // primary poles are marked by add()

  // Prepare room -> worst case
  BOOST_ASSERT( seq.empty() );
  seq.reserve(klasses.size());

#ifndef NDEBUG
  // assert structure
  foreach(klass_t const* k, wanderer.stack)
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
      std::size_t const sz = pseudo_closest(top, &pole);

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
