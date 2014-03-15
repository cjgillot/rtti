#ifndef HEAD_HPP
#define HEAD_HPP

#include "rtti/rtti.hpp"
#include "mmethod/mmethod.hpp"

#include <memory>
#include <boost/mpl/vector.hpp>

using namespace rtti;
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

// namespace rtti {
// 
// namespace __lapin {
// template<typename Ptr, typename T>
// struct shared_ptr_traits
// : traits_detail::smart_ptr_traits<Ptr> {
//   template<typename PU>
//   static PU cast(typename boost::call_traits<Ptr>::param_type p) {
//     typedef typename PU::element_type U;
//     typedef traits_detail::unsafe_casting<U*> caster;
//     return { p, caster::eval( p.get() ) };
//   }
// };
// }
// 
// template<typename T>
// struct pointer_traits<std::shared_ptr<T> >
// : __lapin::shared_ptr_traits<std::shared_ptr<T>, T> {
//   template<typename U>
//   struct rebind {
//     typedef std::shared_ptr<U> other;
//   };
// };
// // template<typename T>
// // struct pointer_traits<std::shared_ptr<T> const>
// // : __lapin::shared_ptr_traits<std::shared_ptr<T> const, T> {
// //   template<typename U>
// //   struct rebind {
// //     typedef std::shared_ptr<U> const other;
// //   };
// // };
// // template<typename T>
// // struct pointer_traits<std::shared_ptr<T> volatile>
// // : __lapin::shared_ptr_traits<std::shared_ptr<T> volatile, T> {
// //   template<typename U>
// //   struct rebind {
// //     typedef std::shared_ptr<U> volatile other;
// //   };
// // };
// // template<typename T>
// // struct pointer_traits<std::shared_ptr<T> const volatile>
// // : __lapin::shared_ptr_traits<std::shared_ptr<T> const volatile, T> {
// //   template<typename U>
// //   struct rebind {
// //     typedef std::shared_ptr<U> const volatile other;
// //   };
// // };
// }

DECLARE_MMETHOD(vf1, void, (_v<foo>*, int));
DECLARE_MMETHOD(vf2, void, (_v<foo>*, _v<foo>&));

#endif
