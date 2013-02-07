#ifndef RTTI_VISITOR_MPL_HPP
#define RTTI_VISITOR_MPL_HPP

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace rtti {
namespace visitor {

//! \brief Miscellanous classes implementing type lists
namespace mpl {

//! \brief Type list construction
template<class H, class T>
struct cons
{
  //! \brief Head type
  typedef H head;
  //! \brief Tail type
  typedef T tail;

  //! \brief List length
  enum { size = 1 + T::size };

  //! \brief Loop through listed types
  template<class F>
  static void for_each(F &f)
  {
    f.template apply<H>();
    T::for_each( f );
  }
};

//! \brief Empty list case
struct nil
{
  //! \brief Null length
  enum { size = 0 };

  //! \brief No-op
  template<class F>
  static void for_each(F &)
  {}
};

// ***** insertion sort for typelist ***** //
// TODO implement a std::list::sort-like algorithm
//   to get a linearithmic behaviour on big lists

// Pred should be a StrictWeakOrdering

template< class T, class TL, class Pred >
struct insert;

template< class T, class H, class TL, class Pred >
struct insert< T, cons<H, TL>, Pred >
{
  enum { is_less = Pred::template apply<T, H>::value };

public:
  typedef typename boost::mpl::if_c< is_less
  , cons< T, cons< H, TL > >
  , cons< H, typename insert< T, TL, Pred >::type >
  >::type type;
};

// ensure type unicity
template< class H, class TL, class Pred >
struct insert< H, cons<H, TL>, Pred >
{
  typedef cons< H, TL > type;
};

template< class T, class Pred >
struct insert< T, nil, Pred >
{
  typedef cons< T, nil > type;
};

template< class TL, class Pred >
struct sort;

template< class H, class TL, class Pred >
class sort< cons< H, TL >, Pred >
{
  typedef typename sort< TL, Pred >::type rec_t;

public:
  typedef typename insert< H, rec_t, Pred >::type type;
};

template< class Pred >
struct sort< nil, Pred >
{
  typedef nil type;
};

} // namespace mpl

}} // namespace rtti::visitor

#endif
