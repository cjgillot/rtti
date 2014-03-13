//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "multifile.hpp"

#include "mmethod/implement.hpp"

struct bar
: foo
, implement_rtti<bar, foo>
{
  int g();
};

int bar::g() { return 42; }
foo* make_bar() { return new bar; }

IMPLEMENT_MMETHOD(f1, int, (bar& a)) { return a.g(); }
