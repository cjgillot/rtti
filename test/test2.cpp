#include "head.hpp"

#include "mmethod/dynamic.tpp"

#include <cstdio>

IMPLEMENT_MMETHOD(vf1, void, (foo*,int)) {}
IMPLEMENT_MMETHOD(vf1, void, (baz*,int)) {}
IMPLEMENT_MMETHOD(vf1, void, (bar*,int)) {}

IMPLEMENT_MMETHOD(vf2, void, (foo*, foo&)) {}
IMPLEMENT_MMETHOD(vf2, void, (baz*, bar&)) {}
IMPLEMENT_MMETHOD(vf2, void, (baz*, baz&)) {}

foo::~foo() {}

void foo::func() {}

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
