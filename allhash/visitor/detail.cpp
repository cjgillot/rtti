#include "rtti/visitor/detail.hpp"
#include "rtti/holder.hpp" // for rtti_node

#include "util/assert.hpp"

#include <algorithm>
#include <functional>

using rtti::rtti_type;
using rtti::detail::rtti_node;
using rtti::visitor::detail::vtable_node;

typedef vtable_node::invoker_t invoker_t;

namespace {

//! \brief lower_bound specialization for vtables
//! \param first range begin
//! \param last range end
//! \param id : the id to be searched
//!
//! The two pointers must form a valid non-empty <em>[first, last)</em> range.
//!
//! \return \c ret verifying <em>[first,ret) < id</em> and <em>*ret > id</em>
inline const vtable_node*
search(
  const vtable_node* first
, const vtable_node* last
, const rtti_type id
) {
  ASSERT( first < last );
  std::size_t len = std::size_t( last - first );

  while( len != 0 )
  {
    const std::size_t half = len >> 1;
    const vtable_node* const middle = first + half;

    if( middle->id < id )
    {
      first = middle + 1;
      len  -= half + 1;
    }
    else
      len   = half;
  }
  return first;
}

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

invoker_t
rtti::visitor::detail::fetch_node(
  const vtable_node beg[]
, const std::size_t vsz
, const rtti_node* rt
) throw() {
  //! issue a binary search for id in the \c vtable,
  //! with an upcast for each failure

  //! worst case time : min( vsz, h ) * lg( vsz )
  //!   with h the inheritance heigth

  //! best case time : 1

  //! usual case time : less than lg( vsz )
  //!   (binary search is only rarely reached, and looping even more)

  // macros stopping iteration
#define TRIVIAL_SUCCESS() \
  do {                    \
    TRIVIAL();            \
    return last->invoker; \
  } while(0) // ;

#define TRIVIAL_FAILURE() \
  do {                    \
    TRIVIAL();            \
    return beg->invoker;  \
  } while(0) // ;

#define SUCCESS() \
  return last->invoker // ;

#define FAILURE() \
  return beg->invoker // ;

  register rtti_type id = rt->id;

  // trivial case
  if( id == 0 ) TRIVIAL_FAILURE();

  // minimal search area
  // because ids are unique
  const vtable_node* last =
    ( id < vsz )
  ? ( beg + id )
  : ( beg + vsz - 1 );

  register rtti_type vid = last->id;

  if( vid == id ) TRIVIAL_SUCCESS();

  //! \invariant:
  //!   \c beg < \c last         [beg, last] is a valid range
  //!   \c rt && \c rt->id != 0  fallback  case not reached
  //! worst case loop termination :
  //!   there is a finite number of upcasts from \c rt up to the base class
  //!   and [beg, last] is a finite range
  //! usual loop termination :
  //!   something found in early steps
  for(;;)
  {
    // singleton range
    if( vid == 0 ) FAILURE();

    // upcast if possible
    while( vid < id ) {
      rt = rt->base;
      id = rt->id;

      UPCAST();

      // base class reached
      if( id == 0 ) FAILURE();
    }

    // match found
    if( vid == id ) SUCCESS();

    // we have id < vid, shrink range [beg, last) -> [beg, new last]
    last = search( beg, last, id );
    vid = last->id;

    SEARCH();

    // got it !
    if( vid == id )  SUCCESS();

    // no match : last - beg <= 1
    //   last == beg => FAILURE()
    //   last == beg+1 and doesn't match => fallback on beg => FAILURE
    if( ! ( (last - beg) >> 1) ) FAILURE();

    --last;
    vid = last->id;
    // search invariant : vid < id

    // upcast once
    rt = rt->base;
    id = rt->id;
    if( id == 0 ) FAILURE();

    LOOP();
  }

#undef HIT
#undef FAILURE

}

namespace {

//! \brief Sorting predicate : with respect to the id
struct sort_pred
: std::binary_function<bool, const vtable_node&, const vtable_node&>
{
  inline bool ATTRIBUTE_PURE
  operator()( const vtable_node &a, const vtable_node &b )
  { return a.id < b.id; }
};

}

namespace std {

template<>
inline void
iter_swap<vtable_node*, vtable_node*>( vtable_node* a, vtable_node* b )
{
  std::swap( a->id, b->id );
  std::swap( a->invoker, b->invoker );
}

}

void
rtti::visitor::detail::sort_nodes(
  vtable_node* const beg
, const std::size_t vsz
) throw() {

  sort_pred pred;
  std::sort( beg, beg + vsz, pred );
}
