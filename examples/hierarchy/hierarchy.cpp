//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "rtti/rtti.hpp"

#include <iostream>

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

struct foo
: base_rtti<foo, 10> {
public:
  virtual ~foo() {}
};

const int bar_id = 6;

struct bar
: foo
, static_rtti<bar, foo, bar_id>
{};

struct baz
: bar
, implement_rtti<baz, bar>
{};

int main() {
  baz z;

  std::cout << "Nodes :" << std::endl;
  std::cout << "- [foo] : " << (void*)static_node<foo>() << " id = " << static_id<foo>() << std::endl;
  std::cout << "- [bar] : " << (void*)static_node<bar>() << " id = " << static_id<bar>() << std::endl;
  std::cout << "- [baz] : " << (void*)static_node<baz>() << " id = " << static_id<baz>() << std::endl;

  // static_id<> is a shorhand for static_node<>()->id
  BOOST_ASSERT( static_node<foo>()->id == static_id<foo>() );

  // explore hierarchy
  rtti_hierarchy h = get_node(z);
  
  // the node is inlined in baz
  BOOST_ASSERT( h       != static_node<baz>()       );
  BOOST_ASSERT( h->id   == static_node<baz>()->id   );
  BOOST_ASSERT( h->base == static_node<baz>()->base );

  // hierarchy traversal
  for(int k = 0; h; ++k, h = h->base)
    std::cout << "Level " << k << " : " << h->id << std::endl;

  return 0;
}
