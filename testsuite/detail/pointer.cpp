//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/traits/pointer_traits.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/static_assert.hpp>

#include <boost/preprocessor/empty.hpp>

using namespace rtti;

// {{{ toy classes used here
namespace {

struct A {
    virtual ~A() {}
};
struct B : A {};
struct C : A {};
struct D : B, C {};

struct VB: virtual A {};
struct VC: virtual A {};
struct VD: VB, VC {};

} // <anonymous>
// }}}

// {{{ test remove_all
namespace {

using traits_detail::remove_all;

#define CHECK_REMOVE_ALL(cvref)              \
    BOOST_STATIC_ASSERT( boost::is_same<A,  \
        remove_all<A cvref>::type           \
    >::value ) /*;*/

CHECK_REMOVE_ALL(BOOST_PP_EMPTY());
CHECK_REMOVE_ALL(const         );
CHECK_REMOVE_ALL(volatile      );
CHECK_REMOVE_ALL(const volatile);
CHECK_REMOVE_ALL(              *);
CHECK_REMOVE_ALL(const         *);
CHECK_REMOVE_ALL(volatile      *);
CHECK_REMOVE_ALL(const volatile*);
CHECK_REMOVE_ALL(              &);
CHECK_REMOVE_ALL(const         &);
CHECK_REMOVE_ALL(volatile      &);
CHECK_REMOVE_ALL(const volatile&);
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
CHECK_REMOVE_ALL(              &&);
CHECK_REMOVE_ALL(const         &&);
CHECK_REMOVE_ALL(volatile      &&);
CHECK_REMOVE_ALL(const volatile&&);
#endif

CHECK_REMOVE_ALL(*              );
CHECK_REMOVE_ALL(*const         );
CHECK_REMOVE_ALL(*volatile      );
CHECK_REMOVE_ALL(*const volatile);
CHECK_REMOVE_ALL(*              *);
CHECK_REMOVE_ALL(*const         *);
CHECK_REMOVE_ALL(*volatile      *);
CHECK_REMOVE_ALL(*const volatile*);
CHECK_REMOVE_ALL(*              &);
CHECK_REMOVE_ALL(*const         &);
CHECK_REMOVE_ALL(*volatile      &);
CHECK_REMOVE_ALL(*const volatile&);
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
CHECK_REMOVE_ALL(*              &&);
CHECK_REMOVE_ALL(*const         &&);
CHECK_REMOVE_ALL(*volatile      &&);
CHECK_REMOVE_ALL(*const volatile&&);
#endif

#undef CHECK_REMOVE_ALL

} // <anonymous>
// }}}

// {{{ test unsafe_casting
BOOST_AUTO_TEST_CASE(test_unsafe_casting) {
  using traits_detail::unsafe_casting;

  B b;
  C c;
  D d;

  VB vb;
  VC vc;
  VD vd;

  A* objs[] = {
      &b, &c, static_cast<B*>(&d), static_cast<C*>(&d),
      &vb, &vc, &vd
  };

  BOOST_CHECK_EQUAL(unsafe_casting< B*>::eval(objs[0]), &b);
  BOOST_CHECK_EQUAL(unsafe_casting< C*>::eval(objs[1]), &c);

  /* should not compile
  BOOST_CHECK_EQUAL(unsafe_casting< D*>::eval(objs[2]), &d);
  BOOST_CHECK_EQUAL(unsafe_casting< D*>::eval(objs[3]), &d);
   */

  BOOST_CHECK_EQUAL(unsafe_casting<VB*>::eval(objs[4]), &vb);
  BOOST_CHECK_EQUAL(unsafe_casting<VC*>::eval(objs[5]), &vc);
  BOOST_CHECK_EQUAL(unsafe_casting<VD*>::eval(objs[6]), &vd);
}
// }}}

// {{{ test pointer_traits
template<typename Klass, typename Type>
void test_type(Type a, Klass* p) {
  typedef typename compute_pointer_traits<Type>::type traits;
  BOOST_STATIC_ASSERT(boost::is_same<
    typename traits::class_type
  , Klass
  >::value);

#define MAKE() (static_cast<Type>(a))
  BOOST_CHECK(traits::valid(MAKE()));
  BOOST_CHECK_EQUAL(&traits::get(MAKE()), p);
#undef MAKE
}

BOOST_AUTO_TEST_CASE(test_basic_pointer_traits) {
  { // non-volatile test
    D d;

    D* dp = &d;
    test_type<D, D*      >(dp, &d);
    test_type<D, D*     &>(dp, &d);
    test_type<D, D*const&>(dp, &d);

    test_type<D, D      &>( d, &d);
    test_type<D, D const&>( d, &d);

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    test_type<D, D     &&>(std::move(d), &d);
#endif
  }

  { // volatile test
    D d;

    D volatile* dp = &d;
    test_type<D, D volatile*      >(dp, &d);
    test_type<D, D volatile*     &>(dp, &d);
    test_type<D, D volatile*const&>(dp, &d);

    test_type<D, D volatile      &>( d, &d);
    test_type<D, D volatile const&>( d, &d);

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    test_type<D, D volatile     &&>(std::move(d), &d);
#endif
  }
}
// }}}
