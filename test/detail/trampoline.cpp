//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/declare/trampoline.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/static_assert.hpp>

using namespace rtti;
using boost::mpl::vector;

// {{{ toy classes used here
namespace {

struct A {};
struct B {};

} // namespace

namespace rtti {

template<>
struct pointer_traits<A> {
  template<typename Out>
  static Out cast(A /*unused*/)
  { return Out(); }
};

} // namespace rtti
// }}}

struct tag_type;
struct ret_type {};
struct exn_type : std::exception {};

struct unary {
  ret_type call(B /*unused*/) {
    BOOST_THROW_EXCEPTION( exn_type() );
  }
};

struct formal {
  ret_type call(B /*unused*/, int i) {
    BOOST_CHECK_EQUAL(i, 42);
    BOOST_THROW_EXCEPTION( exn_type() );
  }
};

struct binary {
  ret_type call(B /*unused*/, B /*unused*/) {
    BOOST_THROW_EXCEPTION( exn_type() );
  }
};

struct vover {
  void call(B /*unused*/) {
    BOOST_THROW_EXCEPTION( exn_type() );
  }
};

BOOST_AUTO_TEST_CASE(test_trampoline) {
  using rtti::mmethod::detail::make_trampoline;

  A a;

  { // unary
    typedef make_trampoline<
      void, ret_type,
      vector<A>,
      vector<boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<unary, ret_type, vector<B> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    BOOST_CHECK_THROW( f(a), exn_type );
  }

  { // formal
    typedef make_trampoline<
      void, ret_type,
      vector<A, int>,
      vector<boost::true_type, boost::false_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A, int)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<formal, ret_type, vector<B, int> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    BOOST_CHECK_THROW( f(a, 42), exn_type );
  }

  { // binary
    typedef make_trampoline<
      void, ret_type,
      vector<A, A>,
      vector<boost::true_type, boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A, A)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<binary, ret_type, vector<B, B> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    BOOST_CHECK_THROW( f(a, a), exn_type );
  }

  { // void
    typedef make_trampoline<
      void, void,
      vector<A>,
      vector<boost::true_type, boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, void(*)(A)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, void
    >::value);
    typedef trampoo_t::apply<vover, void, vector<B> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    BOOST_CHECK_THROW( f(a), exn_type );
  }
}
// }}}
