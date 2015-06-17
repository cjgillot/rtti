//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[po_symm
/*`
  [po_symm_policy]
  [po_symm_decl]
  [po_symm_use]
 */
//]

#include "./classes.hpp"
#include "foreach.hpp"

#include <boost/make_shared.hpp>
#include <boost/test/unit_test.hpp>

#include <iostream>

using namespace rtti;

namespace {

using boost::mpl::vector;

// supplementary class
struct la1
: foo
, implement_rtti<la1, vector<foo> >
{};

struct la2
: la1
, implement_rtti<la2, vector<la1> >
{};

//[po_symm_policy
/*`
  Here, we would like to implement a symmetric function,
  ie. a function whose behaviour does not depend
  on the order of the parameters.
  One example of this would be the implementation
  of an addition primitive in a number hierarchy.

  Taken directly, this task is not trivial to do.
  For instance, with arity /a/,
  each additional /N+1/[super th] class requires
  to implement /N/ cases, and /a!/ identical functions each time.
  This can be automated with the use of the policy class.

  An additional member in the policy class exists,
  the `make_duplicate()` function.
  It is used to create a `duplicator` object,
  whose job is to duplicate any new overload
  into the symmetric overloads.
 */
struct symmetric_policy
: mmethod::default_policy
{
  // generator of permutations
  static boost::shared_ptr<mmethod::duplicator> make_duplicate();
};

/*`
  Each time an overload is registered to the __multimethod__,
  we need a way to register the symmetric overload too.
  The `duplicator` object we want to use has the following
  signature:
 */
class symmetric_duplicator
: public mmethod::duplicator
{
public:
  // load an initial overload (n,t) as a C array
  void load(size_t n, rtti_hierarchy* t);
  // generate permutations in turn
  bool next();

private:
  // member data
  size_t m_arity;
  rtti_hierarchy* m_array;
};

/*`
  When a new overload is encountered,
  the registration algorithm loads it into the `duplicator`
  by the way of the `load()` method.
  The overload is represented by the C array
  of the rtti nodes of its arguments.

  Then, it inserts the overload, calls `next()`
  and repeats until `next()` returns false.
  In our case, we can use that to generate all
  the permutations of the signature array.
 */
void symmetric_duplicator::load(size_t n, rtti_hierarchy* t)
{
  m_arity = n;
  m_array = t;

  // prepare first permutation
  std::sort(m_array, m_array+m_arity);
}

bool symmetric_duplicator::next()
{
  // permute, and return false is exhausted
  return std::next_permutation(m_array, m_array+m_arity);
}

boost::shared_ptr<mmethod::duplicator> symmetric_policy::make_duplicate() {
  return boost::make_shared<symmetric_duplicator>();
}
//]

//[po_symm_decl
/*`
  Thanks to our policy class,
  we do not need to add all the permutations
  for each new behaviour,
  we only need to do it once.
  The `duplicator` takes care of the rest.
 */
using tags::_v;
DECLARE_MMETHOD_POLICY(symm, int, (_v<foo&>, _v<foo&>), symmetric_policy);

IMPLEMENT_MMETHOD(symm, int, (foo&, foo&)) { return  0; }
IMPLEMENT_MMETHOD(symm, int, (bar&, foo&)) { return 13; }
IMPLEMENT_MMETHOD(symm, int, (bar&, bar&)) { return 21; }

// foo-bar is not defined, but the policy resolves it to bar-foo
//]

// further test
IMPLEMENT_MMETHOD(symm, int, (foo&, la1&)) { return 32; }
IMPLEMENT_MMETHOD(symm, int, (bar&, la1&)) { return 65; }
IMPLEMENT_MMETHOD(symm, int, (la1&, la1&)) { return 11; }

IMPLEMENT_MMETHOD(symm, int, (foo&, la2&)) { return 40; }
IMPLEMENT_MMETHOD(symm, int, (bar&, la2&)) { return 45; }
IMPLEMENT_MMETHOD(symm, int, (la1&, la2&)) { return 49; }
IMPLEMENT_MMETHOD(symm, int, (la2&, la2&)) { return 12; }

} // namespace <>

BOOST_AUTO_TEST_CASE(test_symmetric) {
  foo a; bar b;
  foo x; bar y;

  //[po_symm_use
  /*`
    Finally, all this yields a naturally
    symmetric function to the end user.
   */
  BOOST_CHECK_EQUAL( symm(a, x),  0 ); // (1-1 case)
  BOOST_CHECK_EQUAL( symm(a, y), 13 ); // (1-2 case)
  BOOST_CHECK_EQUAL( symm(b, x), 13 ); // (2-1 case)
  BOOST_CHECK_EQUAL( symm(b, y), 21 ); // (2-2 case)
  //]

  la1 z, c;
  BOOST_CHECK_EQUAL( symm(a, z), 32 ); // (1-3 case)
  BOOST_CHECK_EQUAL( symm(c, x), 32 ); // (3-1 case)
  BOOST_CHECK_EQUAL( symm(b, z), 65 ); // (2-3 case)
  BOOST_CHECK_EQUAL( symm(c, y), 65 ); // (3-2 case)
  BOOST_CHECK_EQUAL( symm(c, z), 11 ); // (3-3 case)

  la2 t, d;
  BOOST_CHECK_EQUAL( symm(a, t), 40 ); // (1-4 case)
  BOOST_CHECK_EQUAL( symm(d, x), 40 ); // (4-1 case)
  BOOST_CHECK_EQUAL( symm(b, t), 45 ); // (2-4 case)
  BOOST_CHECK_EQUAL( symm(d, y), 45 ); // (4-2 case)
  BOOST_CHECK_EQUAL( symm(c, t), 49 ); // (2-4 case)
  BOOST_CHECK_EQUAL( symm(d, z), 49 ); // (4-2 case)
  BOOST_CHECK_EQUAL( symm(d, t), 12 ); // (4-4 case)

  foo* arr1 [] = { &a, &b, &c, &d };
  foo* arr2 [] = { &x, &y, &z, &t };

  foreach(foo* o1, arr1)
    foreach(foo* o2, arr2)
      BOOST_CHECK_EQUAL(symm(*o1, *o2), symm(*o2, *o1));
}
