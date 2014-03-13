//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "rtti/rtti.hpp"

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
 * \c bar has a statically defined id \c bar_id
 * \c baz and \c lap respectively inherit from \c foo and \c bar
 */

// Maximal statically assigned ID (strict maximum)
const int max_id = 10;

// ID assigned to class bar
const int bar_id = 6;

using boost::mpl::vector;

struct foo
: rtti::base_rtti<foo, max_id> {
public:
  virtual ~foo() {}
};

struct bar
: foo
, rtti::static_rtti<bar, vector<foo>, bar_id>
{};

struct baz
: foo
, rtti::implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, rtti::implement_rtti<lap, vector<bar> >
{};

int main() {
  foo f; bar r; baz z; lap l;

  std::cout << "Classes IDs :" << std::endl;
  std::cout << "- [foo] base class has id 0 : " << rtti::static_id<foo>() << std::endl;
  std::cout << "- [bar] has known id "          << bar_id << " : " << rtti::static_id<bar>() << std::endl;
  std::cout << "- [baz] class has id : "        << rtti::static_id<baz>() << std::endl;
  std::cout << "- [lap] class has id : "        << rtti::static_id<lap>() << std::endl;

  std::cout << "Objects IDs :" << std::endl;
  std::cout << "- [foo] base class has id 0 : " << rtti::get_id(f) << std::endl;
  std::cout << "- [bar] has known id "          << bar_id << " : " << rtti::get_id(r) << std::endl;
  std::cout << "- [baz] class has id : "        << rtti::get_id(z) << std::endl;
  std::cout << "- [lap] class has id : "        << rtti::get_id(l) << std::endl;

  return 0;
}
