//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"
#include "mmethod/mmethod.hpp"
#include "mmethod/implement.hpp"
#include "mmethod/policy.hpp"

#include <boost/mpl/vector.hpp>

using namespace rtti;

namespace policy_test {

//[po_classes
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{};
//]

} // namespace policy_test

using namespace policy_test;

