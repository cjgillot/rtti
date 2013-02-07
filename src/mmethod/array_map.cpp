#if 0

#include "rtti/mmethod/array_map.hpp"
#include "rtti/holder.hpp" // for rtti_node

#include "util/assert.hpp"

#include <algorithm>
#include <functional>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::mmethod::detail::pole_t;

#define BINARY_SEARCH_THRESHOLD 16

//! \brief linear search for vtables
//! \param first range begin
//! \param last range end
//! \param id : the id to be searched
//!
//! The two pointers must form a valid non-empty <em>[first, last)</em> range.
//!
//! \return \c ret verifying <em>[first,ret) < id</em> and <em>*ret >= id</em>
static inline __attribute__((hot))
const pole_t* ATTRIBUTE_PURE
unguarded_linear_search(
  const pole_t* first
, const rtti_type id
) noexcept {
  while(first->id < id) ++first;
  return first;
}

//! \brief lower_bound specialization for vtables
//! \param first range begin
//! \param last range end
//! \param id : the id to be searched
//!
//! The two pointers must form a valid non-empty <em>[first, last)</em> range.
//!
//! \return \c ret verifying <em>[first,ret) < id</em> and <em>*ret >= id</em>
static inline __attribute__((hot))
const pole_t* ATTRIBUTE_PURE
search(
  const pole_t* first
, const pole_t* last
, const rtti_type id
) noexcept {
  ASSERT( first < last );
  std::size_t len = (std::size_t)std::distance(first, last);

  while( len > BINARY_SEARCH_THRESHOLD ) {
    const std::size_t half = len / 2;
    const pole_t* const middle = first + half;

    if( middle->id < id )
    {
      first = middle + 1;
      len  -= half + 1;
    }
    else
      len   = half;
  }
  return unguarded_linear_search(first, id);
}

// statistic tools
#if 0

struct statistic_t {
  std::size_t nb_trivial;
  std::size_t nb_upcast;
  std::size_t nb_search;
  std::size_t nb_loop;
} statistic;

#define TRIVIAL() ++statistic.nb_trivial
#define UPCAST()  ++statistic.nb_upcast
#define SEARCH()  ++statistic.nb_search
#define LOOP()    ++statistic.nb_loop

#else

#define TRIVIAL()
#define UPCAST()
#define SEARCH()
#define LOOP()

#endif

std::uintptr_t __attribute__((hot)) ATTRIBUTE_PURE
rtti::mmethod::detail::do_fetch_pole(
  const pole_t beg[]
, const std::size_t vsz
, const rtti_node* rt
) noexcept {
  //! issue a binary search for id in the \c vtable,
  //! with an upcast for each failure

  //! worst case time : min( vsz, h ) * lg( vsz )
  //!   with h the inheritance heigth

  //! best case time : 1

  //! usual case time : less than lg( vsz )
  //!   (binary search is only rarely reached, and looping even rarelier)

  // macros stopping iteration
#define TRIVIAL_SUCCESS() \
  do {                    \
    TRIVIAL();            \
    SUCCESS();            \
  } while(0) // ;

#define TRIVIAL_FAILURE() \
  do {                    \
    TRIVIAL();            \
    FAILURE();            \
  } while(0) // ;

#define SUCCESS() \
  return last->data // ;

#define FAILURE() \
  return beg->data // ;

  rtti_type id = rt->id;

  // minimal search area
  // because ids are unique
  const pole_t* last = beg + std::min(id, vsz-1);

  rtti_type vid = last->id;

  //! \invariant:
  //!   \c beg < \c last         [beg, last] is a valid range
  //!   \c rt && \c rt->id != 0  fallback  case not reached
  //! worst case loop termination :
  //!   there is a finite number of upcasts from \c rt up to the base class
  //!   and [beg, last] is a finite range
  //! usual loop termination :
  //!   something found in early steps
  for(;; LOOP())
  {
    // singleton range
    if( vid == 0 ) FAILURE();

    // upcast if possible
    while( vid < id ) {
      rt = rt->base;
      id = rt->id;

      UPCAST();
    }
    // match found
    if( vid == id ) SUCCESS();

    // base class reached
    if( id == 0 ) FAILURE();

    // we have id < vid, shrink range [beg, last) -> [beg, new last]
    last = search( beg, last, id );
    vid = last->id;

    SEARCH();

    // got it !
    if( vid == id )  SUCCESS();

    // no match : last - beg <= 1
    //   last == beg => FAILURE()
    //   last == beg+1 and doesn't match => fallback on beg => FAILURE
    if( (std::size_t)std::distance(beg, last) <= 1 ) FAILURE();

    --last;
    vid = last->id;
    // search invariant : vid < id
  }

  assert(false);
}

void
rtti::mmethod::detail::sort_poles(
  pole_t* const beg,
  const size_t vsz
) throw() {
  std::sort(
    beg, beg + vsz,
    [](const pole_t& a, const pole_t& b)
    { return a.id < b.id; }
  );
}

#endif
