#include "rtti_allhash/multimethod/register.hpp"

#include "rtti_allhash/multimethod/print.hpp"
#include <rtti_allhash/hash.hpp>

#include <rtti_allhash/macro.hpp>

#include <time.h>
#include <iostream>

struct foo       { DECLARE_RTTI(foo, 100); public: virtual void func() {} virtual ~foo() {} };
struct baz : foo { STATIC_RTTI(baz, foo, 2); public: };

#define DECL(name, args) DECLARE_MMETHOD((void), name, args)
#define IMPL(name, args) IMPLEMENT_MMETHOD((void), name, args)

#if 0
DECLARE_MMETHOD(
  (int)
, foo
, (virtual(A const&)(arg0))(static(int)(u))(virtual(A*)(z))
)
IMPLEMENT_MMETHOD(
  (int)
, foo
, (virtual(C const&)(a))(static(int)(u))(virtual(C*)(z))
) {
  return a.a() * u + z->a();
}
#endif

DECL(vf1, (virtual(foo&)()))
IMPL(vf1, (virtual(baz&)())) {}

DECL(vf2, (virtual(foo&)())(virtual(foo&)()))
IMPL(vf2, (virtual(baz&)())(virtual(baz&)())) {}

DECL(vf3, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)()))
IMPL(vf3, (virtual(baz&)())(virtual(baz&)())(virtual(baz&)())) {}

DECL(vf4, (virtual(foo&)())(virtual(foo&)())(virtual(foo&)())(virtual(foo&)()))
IMPL(vf4, (virtual(baz&)())(virtual(baz&)())(virtual(baz&)())(virtual(baz&)())) {}
