//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "multifile.hpp"

#include "boost/mmethod/implement.hpp"

int foo::f() { return 5; }
foo* make_foo() { return new foo; }

BOOST_MMETHOD_IMPLEMENT(f1, int, (foo& a)) { return a.f(); }
