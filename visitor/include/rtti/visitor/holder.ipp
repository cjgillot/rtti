#ifndef RTTI_VISITOR_HOLDER_IPP
#define RTTI_VISITOR_HOLDER_IPP

#include "util/attribute.hpp"

#include "rtti/visitor.hpp"
#include "rtti/getter.hpp"

#include "rtti/visitor/detail.hpp"

#include <algorithm>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace rtti {
namespace visitor {
namespace detail {

template< class Visitor, class Base >
struct invoker {
  //! \brief The function pointer type
  typedef void(*type)( const Visitor&, const Base& );
};


// ***** vtable initialization stuff ***** //

//! \brief Helper structure for initialization
//! This structure is a POD aggregate, to ease creation
template<class Visitor, class Base>
struct init_helper {
  STATIC_ASSERT( boost::is_const<Visitor>::value );
  STATIC_ASSERT( boost::is_const<Base>::value );

  //! \brief Current node in the table
  vtable_node* cur;

  //! \brief Called function
  template<class T>
  inline void apply()
  {
    cur->id = RTTI_TYPE_ID( T );

    // Trampoline implemented as local class
    struct trampoline
    {
      typedef typename boost::remove_const<Visitor>::type ncv_t;
      static void invoker( const Visitor &v, const Base &b )
      { const_cast<ncv_t&>(v).visit( static_cast<const T&>( b ) ); }
    };

    // check function type compatibility
    // this ensures there won't be undefined behaviour
    // when casting back in holder::visit()
    // (ub may be triggered by inconsistent invoker definition change)
    typename invoker< Visitor, Base >::type fnc = &trampoline::invoker;
    cur->invoker = reinterpret_cast< vtable_node::invoker_t >( fnc );

    ASSERT( cur->invoker );
    ++cur;
  }
};

//   abstract type < common type < final type
template<class> class pre_sort;

template<class H, class T>
class pre_sort<mpl::cons<H,T> > {
private:
  // A < B iff :
  //    A is abstract and B is not
  // or B is final and A is not

  // case 1
  enum {
    is_abs = RTTI_GETTER::traits<H>::abstract
  , is_fin = RTTI_GETTER::traits<H>::final
  , is_com = ( ! is_abs ) && ( ! is_fin )
  };

  STATIC_ASSERT( ! (is_abs && is_fin) );

  typedef pre_sort<T> rec_t;
  typedef typename rec_t::abstract rec_abs;
  typedef typename rec_t::common   rec_com;
  typedef typename rec_t::final    rec_fin;

public:
  typedef typename boost::mpl::if_c<
    is_abs
  , mpl::cons<H, rec_abs>
  , rec_abs
  >::type abstract;

  typedef typename boost::mpl::if_c<
    is_fin
  , mpl::cons<H, rec_fin>
  , rec_fin
  >::type final;

  typedef typename boost::mpl::if_c<
    is_com
  , mpl::cons<H, rec_com>
  , rec_com
  >::type common;
};

template<>
class pre_sort<mpl::nil> {
public:
  typedef mpl::nil abstract;
  typedef mpl::nil final;
  typedef mpl::nil common;
};

// ***** initialize function ***** //

template< class Visitor >
inline void
holder<Visitor>::initialize() {
  typedef typename RTTI_GETTER::traits<typename Types::head>::base Base;

  typedef detail::pre_sort<Types> Sort;

  typedef detail::init_helper< const Visitor, const Base > helper_t;
  helper_t h = { vtable };
  Sort::abstract::for_each( h );
  Sort::common  ::for_each( h );
  Sort::final   ::for_each( h );

  rtti::visitor::detail::sort_nodes( vtable, N );

  // assert_initialized tests this field
  ASSERT( vtable[0].invoker );
}

template<class V>
inline void
holder<V>::assert_initialized() {
  initializer.touch();
  if( vtable[0].invoker == 0 )
    initialize();
}

// compound initializer ensures a zero-initialization
// before any non-trivial constructor is called
template<class V>
vtable_node
  holder<V>::vtable[ N ] = {};

template<class V>
typename holder<V>::initializer_t
  holder<V>::initializer;

// ***** visitation implementation ***** //

template< class Visitor >
template< class Base >
inline void
holder<Visitor>::visit( const Visitor &v, const Base &x )
{
  holder::assert_initialized();

  vtable_node::invoker_t inv = rtti::visitor::detail::fetch_node(
    holder::vtable, holder::N, RTTI_NODE( x )
  );

  typedef typename invoker< Visitor, Base >::type fnc_t;
  fnc_t fnc = reinterpret_cast<fnc_t>( inv );

  (*fnc)( v, x );
}


}}} // namespace rtti::visitor::detail

#endif
