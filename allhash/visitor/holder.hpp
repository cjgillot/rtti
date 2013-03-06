#ifndef RTTI_VISITOR_HOLDER_HPP
#define RTTI_VISITOR_HOLDER_HPP

#include "rtti/visitor/detail.hpp"

#include <boost/type_traits/is_const.hpp>

#include "util/assert.hpp"

namespace rtti {
namespace visitor {
namespace detail {

//! \brief Instanciation class for visitor states
template< class Visitor >
struct holder {

public:
  //! \brief Initialization routine
  static void assert_initialized();

  //! \brief Visit method
  template< class Base >
  static void visit(
    const Visitor &v
  , const Base &x
  );

private: // implementation
  //! \invariant the template parameter is const-qualified
  STATIC_ASSERT( boost::is_const< Visitor >::value );

  typedef typename Visitor::visited_type Types;

  //! \brief Vtable size
  enum { N = Types::size };
  STATIC_ASSERT( N > 0 );

  //! \brief Vtable initialization method
  //! This method is meant to be called by
  //! \c assert_initialized to avoid repeated initialization.
  //! Compile time complexity : O( N )
  //! Runtime complexity : O( N * lg N ) : std::sort the vtable
  static void
  initialize();

  struct initializer_t {
    initializer_t() { holder::assert_initialized(); }
    void touch()    {}
  } static initializer;

private: // vtable data
  //! \brief Vtable static symbol
  static detail::vtable_node vtable [ N ];
};

} // namespace detail

template< class Visitor >
struct get_holder {
  typedef ::rtti::visitor::detail::holder< const Visitor > type;
};

}} // namespace rtti::visitor

#endif

#include "rtti/visitor/holder.ipp"
