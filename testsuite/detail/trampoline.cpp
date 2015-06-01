//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/declare/trampoline.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

// {{{ toy classes used here
namespace {

struct A {
    virtual ~A() {}
};
struct B : A {};

} // <anonymous>
// }}}

struct tag_type;
struct ret_type {};

bool unary_called = false;
struct unary {
  ret_type call(B* b) {
    unary_called = true;
    BOOST_CHECK(b);
    return ret_type();
  }
};

bool formal_called = false;
struct formal {
  ret_type call(B* b, int i) {
    formal_called = true;
    BOOST_CHECK(b);
    BOOST_CHECK_EQUAL(i, 42);
    return ret_type();
  }
};

bool binary_called = false;
struct binary {
  ret_type call(B* b1, B* b2) {
    binary_called = true;
    BOOST_CHECK(b1);
    BOOST_CHECK(b2);
    return ret_type();
  }
};

bool void_called = false;
struct vover {
  void call(B* b) {
    void_called = true;
    BOOST_CHECK(b);
  }
};

BOOST_AUTO_TEST_CASE(test_trampoline) {
  using rtti::mmethod::detail::make_trampoline;

  B b;
  A* a = &b;

  { // unary
    typedef make_trampoline<
      void, ret_type,
      vector<A*>,
      vector<boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A*)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<unary, ret_type, vector<B*> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    f(a);
  }

  { // formal
    typedef make_trampoline<
      void, ret_type,
      vector<A*, int>,
      vector<boost::true_type, boost::false_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A*, int)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<formal, ret_type, vector<B*, int> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    f(a, 42);
  }

  { // binary
    typedef make_trampoline<
      void, ret_type,
      vector<A*, A*>,
      vector<boost::true_type, boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, ret_type(*)(A*, A*)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, ret_type
    >::value);
    typedef trampoo_t::apply<binary, ret_type, vector<B*, B*> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    f(a, a);
  }

  { // void
    typedef make_trampoline<
      void, void,
      vector<A*>,
      vector<boost::true_type, boost::true_type>
    > trampoo_t;
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::sig_t, void(*)(A*)
    >::value);
    BOOST_STATIC_ASSERT(boost::is_same<
      trampoo_t::return_type, void
    >::value);
    typedef trampoo_t::apply<vover, void, vector<B*> > apply_t;

    trampoo_t::sig_t f = &apply_t::call;

    BOOST_CHECK_EQUAL( reinterpret_cast<intptr_t>(f) & 1, 0 );
    f(a);
  }
}
// }}}
