//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[un_unary
/*`
  [un_hier]
  [un_decl]
  [un_impl]
  [un_use]
 */
//]

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace {

//[un_hier
/*`
  Before declaring a __multimethod__, we will need a
  __rtti__-enabled class hierarchy.
 */

using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f() const { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() const { return 42; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};
//]

//[un_decl
/*`
  The declaration of a __multimethod__ is done via
  the `DECLARE_MMETHOD` macro. This macro takes three arguments :
  * the __multimethod__ name
  * the return type
  * the parenthesized parameter type list

  There can be two kinds of parameters : the neutral parameters
  and the dispatched parameters.
  Neutral parameters are just passed as-is to the called implementation.
  Dispatched parameters are those on which the multiple dispatch is computed.
  In order to differentiate between these, dispatched parameters must be tagged
  using the `tags::_v<>` template.
 */
using tags::_v;
DECLARE_MMETHOD(f1, int, (_v<foo const&>));
/*`
  Here, the sole argument is a dispatched parameter.
  
  The dependency on __rtti__ appears : all dispatched parameters
  must have been registered with __rtti__ prior to the declaration.
 */
//]

//[un_impl
/*`
  After a __multimethod__ declaration, the `IMPLEMENT_MMETHOD` macro
  can be used to provide actual code for our __multimethod__.
  The parameters are like `DECLARE_MMETHOD` :
  * the __multimethod__ name
  * the return type
  * the parameter list
  
  Note that unlike the `DECLARE_MMETHOD` macro,
  no tagging is required on the parameters.
 */
IMPLEMENT_MMETHOD(f1, int, (foo const& a)) { return a.f(); }
IMPLEMENT_MMETHOD(f1, int, (bar const& a)) { return a.g(); }
IMPLEMENT_MMETHOD(f1, int, (baz const& a)) { return 2 * a.f(); }
//]

} // namespace <>

BOOST_AUTO_TEST_CASE(unary) {
  //[un_use
  /*`
    Our __multimethod__ can now be used as a function object,
    all the dispatch is done transparently for the user.
   */
  foo f; bar r; baz z; lap l;

  BOOST_CHECK_EQUAL( f1(f),  5 );
  BOOST_CHECK_EQUAL( f1(r), 42 );
  BOOST_CHECK_EQUAL( f1(z), 10 );
  BOOST_CHECK_EQUAL( f1(l), 42 ); // (lap is-a bar)
  //]
}
