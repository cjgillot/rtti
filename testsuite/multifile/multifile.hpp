//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MULTIFILE_HPP
#define MULTIFILE_HPP

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/declare.hpp"

using namespace boost::mmethod;

namespace multifile {

struct foo
: base_rtti<foo> {
public:
  virtual ~foo();

  int f();
};

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<foo&>));

foo* make_foo();
foo* make_bar();
foo* make_baz();

} // namespace multifile

#endif
