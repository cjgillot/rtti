//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "mmethod/traits/pointer_traits.hpp"

#include <boost/type_traits/is_same.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/static_assert.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/cast.hpp>

using namespace rtti;

namespace rtti {

// begin partial specialization
template<typename T>
struct pointer_traits<boost::shared_ptr<T> > {
  typedef boost::shared_ptr<T> pointer_type;

  typedef typename boost::remove_cv<T>::type class_type;

  static T&     get(pointer_type const& v) { return *v; }
  static bool valid(pointer_type const& v) { return bool(v); }

  template<typename U>
  static typename rtti::traits_detail::remove_all<U>::type
  cast(pointer_type const& v) {
    typedef typename rtti::traits_detail::remove_all<U>::type Uclass;
    typedef typename Uclass::element_type O;

    BOOST_STATIC_ASSERT(boost::is_same<
        Uclass, boost::shared_ptr<O>
    >::value);

    return Uclass(v, boost::polymorphic_downcast<O*>(v.get()));
  }
};

} // namespace rtti

// {{{ toy classes used here
namespace {

struct A {
    virtual ~A() {}
};
struct B : A {};

} // <anonymous>
// }}}

// {{{ test pointer_traits
BOOST_AUTO_TEST_CASE(test_smart_pointer_traits) {
    boost::shared_ptr<A> ap ( new B );

    typedef pointer_traits<boost::shared_ptr<A> > ptr_tt;
    BOOST_STATIC_ASSERT(boost::is_same<ptr_tt::class_type, A>::value);
    BOOST_CHECK(ptr_tt::valid(ap));
    BOOST_CHECK_EQUAL(&ptr_tt::get(ap), ap.get());

    boost::shared_ptr<B> bp = ptr_tt::cast<boost::shared_ptr<B> >(ap);
    BOOST_CHECK_EQUAL(bp.get(), ap.get());
}
// }}}

