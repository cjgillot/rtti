//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "head.hpp"

#include "boost/mmethod/implement.hpp"

#include <cstdio>

BOOST_MMETHOD_IMPLEMENT(vf1, void, (foo*,int)) {}
BOOST_MMETHOD_IMPLEMENT(vf1, void, (baz*,int)) {}
BOOST_MMETHOD_IMPLEMENT(vf1, void, (bar*,int)) {}

BOOST_MMETHOD_IMPLEMENT(vf2, void, (foo*, foo&)) {}
BOOST_MMETHOD_IMPLEMENT(vf2, void, (baz*, bar&)) {}
BOOST_MMETHOD_IMPLEMENT(vf2, void, (baz*, baz&)) {}

foo::~foo() {}

void foo::func() {}
void bar::func() {}
void baz::func() {}

void loop_virt(foo* f, size_t N) {
  do
    f->func();
  while(--N);
}

void loop_mm1 (foo* f, size_t N) {
  do
    vf1(f,0);
  while(--N);
}

void loop_mm2 (foo* f, foo* g, size_t N) {
  do
    vf2(f, *g);
  while(--N);
}
