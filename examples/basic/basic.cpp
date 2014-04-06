//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"

#include <iostream>
#include <boost/mpl/vector.hpp>

/*!\example basic.cpp
 * 
 * This example demonstrates basic usage of \c rtti.
 * 
 * Four classes are used : \c foo, \c bar, \c baz, \c lap
 * Their ids are then output on stdout
 * 
 * \c foo is the base class
 * \c bar inherits from \c foo
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

using namespace boost::mmethod;
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

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

int main() {
  foo f; bar r; baz z; lap l;

  std::cout << "Classes IDs :" << std::endl;
  std::cout << "- [foo] " << static_id<foo>() << std::endl;
  std::cout << "- [bar] " << static_id<bar>() << std::endl;
  std::cout << "- [baz] " << static_id<baz>() << std::endl;
  std::cout << "- [lap] " << static_id<lap>() << std::endl;

  std::cout << "Objects IDs :" << std::endl;
  std::cout << "- [foo] " << get_id(f) << std::endl;
  std::cout << "- [bar] " << get_id(r) << std::endl;
  std::cout << "- [baz] " << get_id(z) << std::endl;
  std::cout << "- [lap] " << get_id(l) << std::endl;

  return 0;
}
