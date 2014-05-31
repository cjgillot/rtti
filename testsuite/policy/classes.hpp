//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"

#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;

namespace policy_test {

//[po_classes
using boost::mpl::vector;

struct foo1
: base_rtti<foo1> {
public:
  virtual ~foo1() {}
};

struct foo2
: foo1
, implement_rtti<foo2, vector<foo1> >
{};

struct bar1
: base_rtti<bar1> {
public:
  virtual ~bar1() {}
};

struct bar2
: bar1
, implement_rtti<bar2, vector<bar1> >
{};
//]

} // namespace policy_test

using namespace policy_test;

