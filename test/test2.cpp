#include "rtti.hpp"
#include "rtti/mmethod.hpp"
#include "rtti/mmethod.tpp"

#include <cstdio>

struct foo
: DECLARE_RTTI(foo, 100) {
public:
  virtual void func();
  virtual ~foo();
};

struct bar
: foo
, STATIC_RTTI(bar, foo, 5) {};

struct baz
: bar
, IMPLEMENT_RTTI(baz, bar) {};

template<typename T>
using v_ = rtti::mmethod::tags::virtual_<T>;
DECLARE_MMETHOD(vf1, void, (v_<foo>*));

IMPLEMENT_MMETHOD(vf1, void, (foo*)) {}
IMPLEMENT_MMETHOD(vf1, void, (baz*)) {}

template<typename T>
using v_ = rtti::mmethod::tags::virtual_<T>;
DECLARE_MMETHOD(vf2, void, (v_<foo>*, v_<foo>&));

IMPLEMENT_MMETHOD(vf2, void, (foo*, foo&)) { /*printf("lapin"); */}
IMPLEMENT_MMETHOD(vf2, void, (baz*, bar&)) { /*printf("canard");*/}
// IMPLEMENT_MMETHOD(vf2, void, (baz*, baz&)) { /*printf("choux"); */}

foo::~foo() {}

void foo::func() {}

void loop_virt(foo* f, size_t N) {
  do f->func();
  while(--N);
}

void loop_mm1 (foo* f, size_t N) {
  do vf1(f);
  while(--N);
}

void loop_mm2 (foo* f, foo* g, size_t N) {
  do vf2(f, *g);
  while(--N);
}
