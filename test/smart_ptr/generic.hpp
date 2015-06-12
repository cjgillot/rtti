//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Expected definitions:
// * PTR(T)
// * TESTNAME
// * CALL(f,x)

using namespace rtti::mmethod;

#define NAMESPACE BOOST_JOIN(TESTNAME,_ns)

namespace NAMESPACE {

using tags::_v;
DECLARE_MMETHOD(smp, int, (_v<PTR(foo) >));

IMPLEMENT_MMETHOD(smp, int, (PTR(foo) a)) { return a->f(); }
IMPLEMENT_MMETHOD(smp, int, (PTR(bar) a)) { return a->g(); }
IMPLEMENT_MMETHOD(smp, int, (PTR(baz) a)) { return 2 * a->f(); }

} // namespace <>

BOOST_AUTO_TEST_CASE(TESTNAME) {
  using namespace NAMESPACE;

  typedef PTR(foo) pointer;
  pointer f ( new foo );
  pointer r ( new bar );
  pointer z ( new baz );
  pointer l ( new lap );

  BOOST_CHECK_EQUAL( CALL(smp,f),  5 );
  BOOST_CHECK_EQUAL( CALL(smp,r), 42 );
  BOOST_CHECK_EQUAL( CALL(smp,z), 10 );
  BOOST_CHECK_EQUAL( CALL(smp,l), 42 ); // (lap is-a bar)
}

#undef NAMESPACE
