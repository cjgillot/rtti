//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLASSES_HPP
#define CLASSES_HPP

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace mmethod_hierarchy {

//[hi_hier
/*`
  Before declaring a __multimethod__, we will need a
  __rtti__-enabled class hierarchy.
  We will keep the following classes for all the examples here.
 */

using boost::mpl::vector;

struct foo
: base_rtti<foo>
, private boost::noncopyable {
public:
  virtual ~foo();

  int f() const { return 5; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{
  virtual ~baz();
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  virtual ~bar();

  int g() const { return 42; }
};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{
  virtual ~lap();
};
//]

} // namespace mmethod_hierarchy
using namespace mmethod_hierarchy;

#endif
