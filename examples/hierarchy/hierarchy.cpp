//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/rtti.hpp"

#include <iostream>
#include <boost/mpl/vector.hpp>

/*!\example hierarchy.cpp
 * 
 * This example demonstrates hierarchy traversal with \c rtti.
 * 
 * Three classes are used : \c foo, \c bar, \c baz
 * Their node's addresses and ids are output on stdout.
 * \c baz upward hierarchy is output on stdout.
 * 
 * \c foo is the base class
 * \c bar has a statically defined id \c bar_id
 * \c baz inherits from \c bar
 */

using namespace rtti;
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
};

const int bar_id = 6;

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{};

struct baz
: bar
, implement_rtti<baz, vector<bar> >
{};

int main() {
  baz z;

  std::cout << "Nodes :" << std::endl;
  std::cout << "- [foo] : " << static_node<foo>() << " id = " << static_id<foo>() << std::endl;
  std::cout << "- [bar] : " << static_node<bar>() << " id = " << static_id<bar>() << std::endl;
  std::cout << "- [baz] : " << static_node<baz>() << " id = " << static_id<baz>() << std::endl;

  // static_id<> is a shorhand for static_node<>()->id
  BOOST_ASSERT( rtti_get_id( static_node<foo>() ) == static_id<foo>() );

  // explore hierarchy
  rtti_hierarchy h = get_node(z);

  // the node is inlined in baz
  BOOST_ASSERT( h                == static_node<baz>()       );
  BOOST_ASSERT( rtti_get_id  (h) == rtti_get_id  (static_node<baz>()) );
  BOOST_ASSERT( rtti_get_base(h) == rtti_get_base(static_node<baz>()) );

  // hierarchy traversal
  for(int k = 0;; ++k) {
    std::cout << "Level " << k << " : " << rtti_get_id(h) << std::endl;
    
    if(rtti_get_base_arity(h))
      h = rtti_get_base(h);
    else
      break;
  }

  return 0;
}
