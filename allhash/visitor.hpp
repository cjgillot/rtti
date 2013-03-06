#ifndef RTTI_VISITOR_HPP
#define RTTI_VISITOR_HPP

#include "rtti/visitor/mpl.hpp"
#include "rtti/visitor/holder.hpp"

#include <boost/preprocessor/seq/for_each.hpp>

namespace rtti {

//! \brief Efficient cyclic visitor based on RTTI
namespace visitor {

template< class Visitor, class Base >
void visit(
  const Visitor &v
, const Base &x
) {
  typedef typename get_holder< Visitor >::type H;
  H::visit( v, x );
}

}} // namespace rtti::visitor

#define MAKE_VISITABLE( klass )   \
  template<class Visitor>         \
  inline void                     \
  accept( const Visitor &v ) const\
  {                               \
    ::rtti::visitor::visit        \
      ( v, *this );               \
  }

#define VISITOR_TLIST( sequence, tail )                     \
  BOOST_PP_SEQ_FOR_EACH( VISITOR_TLIST_BEGIN, _, sequence ) \
    tail                                                    \
  BOOST_PP_SEQ_FOR_EACH( VISITOR_TLIST_END,   _, sequence )

#define VISITOR_TLIST_BEGIN( r, _, elem ) \
  ::rtti::visitor::mpl::cons< elem,

#define VISITOR_TLIST_END( r, _, elem ) >

#define MAKE_VISITOR( klass, base, visited )    \
protected:                                      \
  typedef VISITOR_TLIST(                        \
    (base)visited                               \
  , ::rtti::visitor::mpl::nil                   \
  )                                             \
    visited_type;                               \
  friend class ::rtti::visitor::detail::holder< const klass >;

#define EXTEND_VISITOR( klass, parent, base, visited )    \
protected:                                      \
  typedef VISITOR_TLIST(                        \
    (base)visited                               \
  , typename parent::visited_type               \
  )                                             \
    visited_type;                               \
  friend class ::rtti::visitor::detail::holder< const klass >;

#endif
