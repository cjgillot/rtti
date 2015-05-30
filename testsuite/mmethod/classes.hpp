//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace mmethod_hierarchy {

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

} // namespace mmethod_hierarchy
using namespace mmethod_hierarchy;

