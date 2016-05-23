//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "head.hpp"

#include "mmethod/implement.hpp"

#include <cstdio>

IMPLEMENT_MMETHOD(vf1, void, (foo&,int)) {}
IMPLEMENT_MMETHOD(vf1, void, (baz&,int)) {}
IMPLEMENT_MMETHOD(vf1, void, (bar&,int)) {}

IMPLEMENT_MMETHOD(vf2, void, (foo&, foo&)) {}
IMPLEMENT_MMETHOD(vf2, void, (baz&, bar&)) {}
IMPLEMENT_MMETHOD(vf2, void, (baz&, baz&)) {}

foo::~foo() {}

void foo::func() {}
void bar::func() {}
void baz::func() {}

void loop_virt(foo& f, size_t N) {
  do
    f.func();
  while(--N);
}

void loop_mm1 (foo& f, size_t N) {
  vf1.generate();
  do
    vf1.fast_call(f,0);
  while(--N);
}

void loop_mm2 (foo& f, foo& g, size_t N) {
  vf2.generate();
  do
    vf2.fast_call(f, g);
  while(--N);
}
