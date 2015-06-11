//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[mu_multiple
/*`
  The default behaviour of __rtti__ does not support multiple inheritance.
  This design choice is inspired by a concern about performance
  of node retrieval.

  However, multiple inheritance support can be enabled using
  the second argument to `base_rtti`. This is called the /NodeType/.

  Default /NodeType/ is `single_mixin_node`,
  this is well suited for most use cases,
  but only allows for one __rtti__-enabled parent for each class.
  The other possible /NodeType/ is `virtual_mixin_node`.
  This type allows usage of full C++ inheritance features,
  at the expense of performance.

  Internally, `virtual_mixin_node` uses virtual inheritance (hence the name).
  Therefore the performance will be poorer than with `single_mixin_node`
  using only regular inheritance.

  [mu_decl]

  The chosen /NodeType/ is then remembered by the hierarchy
  for calls to `implement_rtti`.
  [mu_impl]

  [important
    All bases involved in the same hierarchy must use the same /NodeType/.
    Otherwise, your compiler will barf.
  ]
 */
//]

#include "mmethod/rtti.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>

using namespace rtti;
using boost::mpl::vector;

namespace {

//[mu_decl
struct foo
: base_rtti<foo, virtual_mixin_node>
{
public:
  virtual ~foo() {}
};

struct bar
: base_rtti<bar, virtual_mixin_node>
{
public:
  virtual ~bar() {}
};
//]

//[mu_impl
struct baz
: foo, bar
, implement_rtti<baz, vector<foo, bar> >
{};
//]

} // namespace

BOOST_AUTO_TEST_CASE(test_multiple) {
  foo f; bar r; baz z;

  BOOST_CHECK_EQUAL( static_id<foo>(), get_id(f) );
  BOOST_CHECK_EQUAL( static_id<bar>(), get_id(r) );
  BOOST_CHECK_EQUAL( static_id<baz>(), get_id(z) );
}
