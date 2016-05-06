//          Copyright Camille Gillot 2012 - 2016.
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

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iostream>

using namespace rtti;

namespace {

using boost::mpl::vector;
using rtti::mmethod::ambiguity::action_t;

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
  template<typename Tag, typename Args, typename Callback>
  struct duplicates;

  // resolve ambiguities
  static action_t ambiguity_handler(std::size_t, rtti_hierarchy*);
};

/*`
  Each time an overload is registered to the __multimethod__,
  we need a way to register the symmetric overload too.
  The `duplicates` template is used for that.
  The template parameters are:
  - the tag of the __mmethod__,
  - the argument types of the overload,
  - a class containing the callback to the code of hte overload.

  The `duplicates` class must model a __sequence__
  of __sequence__s of two elements:
  each element in `duplicates<...>` is of the form
  `vector< args, callback >`.
  In this case, `args` is the permuted argument type list,
  and `callback` is the class containing the code.
 */
template<typename Tag, typename Args, typename Callback>
class symmetric_policy::duplicates
{
private:
  typedef typename Tag::result_type                return_type;
  typedef typename boost::mpl::at_c<Args, 0>::type el0_type;
  typedef typename boost::mpl::at_c<Args, 1>::type el1_type;

  typedef boost::mpl::vector<el1_type, el0_type> swapped_args;
  struct swapped_callback {
    static return_type call(foo& a, foo& b) {
      return Callback::call(b, a);
    }
  };

  typedef boost::is_same<el0_type, el1_type> is_same;

  typedef boost::mpl::pair<
    Args, Callback
  > first;
  typedef boost::mpl::pair<
    swapped_args, swapped_callback
  > second;

public:
  typedef typename boost::mpl::if_<
    is_same,
    boost::mpl::vector<first>,
    boost::mpl::vector<first, second>
  >::type type;

  BOOST_STATIC_ASSERT(! boost::mpl::empty<type>::value);
};

/*`
  The idea of injecting missing implementations is appealing,
  but suffers one flaw: it generates ambiguities.
  For example, take a call with two identical arguments.
  If we don't the symmetric overload,
  it is correctly resolved by upcasting one argument.
  But if we add the symmetric overload,
  the upcast becomes ambiguous.
 */
action_t
symmetric_policy::ambiguity_handler(std::size_t n, rtti_hierarchy* types)
{ /*<-*/BOOST_ASSERT(n == 2);/*->*/
  if( types[0] != types[1] ) {
    // This is a real ambiguity, report it as such
    return action_t::NOTHING;
  }

  // Can we force the upcast of the argument ?
  if(rtti_get_base_arity(types[0])) {
    types[0] = rtti_get_base(types[0]);
    return action_t::RETRY;
  }

  // This is a root-root ambiguity
  return action_t::NOTHING;
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

// foo-bar is not defined, but the policy resolves it to bar-foo
// bar-bar is not defined, but the policy resolves it to foo-bar
//]

// further test
IMPLEMENT_MMETHOD(symm, int, (foo& a, la1& b)) { return 32; }
IMPLEMENT_MMETHOD(symm, int, (bar& a, la1& b)) { return 65; }
// no la1-la1 -> fallback on foo-la1

IMPLEMENT_MMETHOD(symm, int, (foo& a, la2& b)) { return 40; }
IMPLEMENT_MMETHOD(symm, int, (bar& a, la2& b)) { return 45; }
IMPLEMENT_MMETHOD(symm, int, (la1& a, la2& b)) { return 49; }
IMPLEMENT_MMETHOD(symm, int, (la2& a, la2& b)) { return 12; }

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
  BOOST_CHECK_EQUAL( symm(b, y), 13 ); // (2-2 case)
  //]

  la1 z, c;
  BOOST_CHECK_EQUAL( symm(a, z), 32 ); // (1-3 case)
  BOOST_CHECK_EQUAL( symm(c, x), 32 ); // (3-1 case)
  BOOST_CHECK_EQUAL( symm(b, z), 65 ); // (2-3 case)
  BOOST_CHECK_EQUAL( symm(c, y), 65 ); // (3-2 case)
  BOOST_CHECK_EQUAL( symm(c, z), 32 ); // (3-3 case) - fallback on 1-3

  la2 t, d;
  BOOST_CHECK_EQUAL( symm(a, t), 40 ); // (1-4 case)
  BOOST_CHECK_EQUAL( symm(d, x), 40 ); // (4-1 case)
  BOOST_CHECK_EQUAL( symm(b, t), 45 ); // (2-4 case)
  BOOST_CHECK_EQUAL( symm(d, y), 45 ); // (4-2 case)
  BOOST_CHECK_EQUAL( symm(c, t), 49 ); // (3-4 case)
  BOOST_CHECK_EQUAL( symm(d, z), 49 ); // (4-3 case)
  BOOST_CHECK_EQUAL( symm(d, t), 12 ); // (4-4 case)

  foo* arr1 [] = { &a, &b, &c, &d };
  foo* arr2 [] = { &x, &y, &z, &t };

  foreach(foo* o1, arr1)
    foreach(foo* o2, arr2)
      BOOST_CHECK_EQUAL(symm(*o1, *o2), symm(*o2, *o1));
}
