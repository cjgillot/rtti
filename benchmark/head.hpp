//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef HEAD_HPP
#define HEAD_HPP

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"

#include <memory>
#include <boost/mpl/vector.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual void func();
  virtual ~foo();
};

struct bar
: foo
, implement_rtti<bar, vector<foo> > {
  virtual void func();
};

struct baz
: foo
, implement_rtti<baz, vector<foo> > {
  virtual void func();
};

using tags::_v;

BOOST_MMETHOD_DECLARE(vf1, void, (_v<foo&>, int));
BOOST_MMETHOD_DECLARE(vf2, void, (_v<foo&>, _v<foo&>));

#endif
