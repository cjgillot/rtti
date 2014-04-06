//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "boost/mmethod/rtti.hpp"
#include "boost/mmethod/mmethod.hpp"
#include "boost/mmethod/implement.hpp"

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/cast.hpp>

using namespace boost::mmethod;
using boost::mpl::vector;

struct foo
: base_rtti<foo> {
public:
  virtual ~foo() {}
  
  int f() { return 5; }
};

struct bar
: foo
, implement_rtti<bar, vector<foo> >
{
  int g() { return 42; }
};

struct baz
: foo
, implement_rtti<baz, vector<foo> >
{};

struct lap
: bar
, implement_rtti<lap, vector<bar> >
{};

#define PTR(T) boost::shared_ptr< T >

namespace boost {
namespace mmethod {

template<typename T>
struct pointer_traits<PTR(T)> {
  typedef typename boost::remove_cv<T>::type class_type;

  static T& get(PTR(T) const& v) { return *v; }
  static bool valid(PTR(T) const& v) { return bool(v); }

  template<typename U>
  static typename boost::mmethod::traits_detail::remove_all<U>::type
  cast(PTR(T) v) {
    typedef typename boost::mmethod::traits_detail::remove_all<U>::type Uclass;
    typedef typename Uclass::element_type O;
    return Uclass(v, boost::polymorphic_downcast<O*>(v.get()));
  }
};

}} // namespace boos::mmethod

using tags::_v;
BOOST_MMETHOD_DECLARE(f1, int, (_v<PTR(foo) const&>));

BOOST_MMETHOD_IMPLEMENT(f1, int, (PTR(foo) const& a)) { return a->f(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (PTR(bar) a)) { return a->g(); }
BOOST_MMETHOD_IMPLEMENT(f1, int, (PTR(baz) a)) { return 2 * a->f(); }

int main() {
  PTR(foo) f ( new foo );
  PTR(bar) r ( new bar );
  PTR(baz) z ( new baz );
  PTR(lap) l ( new lap );

  std::cout << f1(f) << std::endl; // prints 5
  std::cout << f1(r) << std::endl; // prints 42
  std::cout << f1(z) << std::endl; // prints 10
  std::cout << f1(l) << std::endl; // prints 42 (lap is-a bar)
  
  return 0;
}
