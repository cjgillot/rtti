#ifndef HEAD_HPP
#define HEAD_HPP

#include "rtti.hpp"
#include "mmethod/static/register.hpp"

#include <memory>

struct foo
: DECLARE_RTTI(foo, 100) {
public:
  virtual void func();
  virtual ~foo();
};

struct bar
: foo
, STATIC_RTTI(bar, foo, 6) {};

struct baz
: foo
, IMPLEMENT_RTTI(baz, foo) {};

template<typename T> using v_ = rtti::tags::virtual_<T>;

namespace rtti {

namespace __lapin {
template<typename Ptr, typename T>
struct shared_ptr_traits
: rtti::traits_detail::smart_ptr_traits<Ptr> {
  template<typename PU>
  static PU cast(typename boost::call_traits<Ptr>::param_type p) {
    typedef typename PU::element_type U;
    typedef rtti::traits_detail::unsafe_casting<U*> caster;
    return { p, caster::eval( p.get() ) };
  }
};
}

template<typename T>
struct pointer_traits<std::shared_ptr<T> >
: __lapin::shared_ptr_traits<std::shared_ptr<T>, T> {
  template<typename U>
  struct rebind {
    typedef std::shared_ptr<U> other;
  };
};
// template<typename T>
// struct pointer_traits<std::shared_ptr<T> const>
// : __lapin::shared_ptr_traits<std::shared_ptr<T> const, T> {
//   template<typename U>
//   struct rebind {
//     typedef std::shared_ptr<U> const other;
//   };
// };
// template<typename T>
// struct pointer_traits<std::shared_ptr<T> volatile>
// : __lapin::shared_ptr_traits<std::shared_ptr<T> volatile, T> {
//   template<typename U>
//   struct rebind {
//     typedef std::shared_ptr<U> volatile other;
//   };
// };
// template<typename T>
// struct pointer_traits<std::shared_ptr<T> const volatile>
// : __lapin::shared_ptr_traits<std::shared_ptr<T> const volatile, T> {
//   template<typename U>
//   struct rebind {
//     typedef std::shared_ptr<U> const volatile other;
//   };
// };
}

DECLARE_MMETHOD(vf1, void, (v_<foo>*, int));
DECLARE_MMETHOD(vf2, void, (v_<foo>*, v_<foo>&&));

#endif
