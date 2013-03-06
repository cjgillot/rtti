#ifndef RTTI_MMETHOD_PRINT_HPP
#define RTTI_MMETHOD_PRINT_HPP

#include <cstdlib>
#include <boost/preprocessor/cat.hpp>

#if defined(__GNUC__)
#define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

namespace detail
{
    struct true_type {};
    struct false_type {};
    template <int test> struct converter : public true_type {};
    template <> struct converter<0> : public false_type {};
}

#define static_warning(cond, msg) \
struct BOOST_PP_CAT(static_warning,__LINE__) { \
  DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
  void _(::detail::true_type const& ) {}; \
  BOOST_PP_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
}

// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
// It introduces a member/variable declaration.  This means at least one byte of space
// in each structure/class instantiation.  STATIC_WARNING should be preferred in any
// non-template situation.
//  'token' must be a program-wide unique identifier.
// #define STATIC_WARNING_TEMPLATE(token, cond, msg)
//     STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

#include <boost/mpl/int.hpp>
#include <boost/mpl/print.hpp>

namespace rtti {

template<typename T>
struct print
: boost::mpl::print<T> {
  print() { char __print = sizeof(T) + 256; (void)__print; }
  using boost::mpl::print<T>::type;
};

#define RTTI_PRINT(arg) \
  struct BOOST_PP_CAT(__rtti_mmethod_print, __LINE__) {       \
    BOOST_PP_CAT(__rtti_mmethod_print, __LINE__)() { ::rtti::print< arg > __u; (void)__u; }   \
  } /*;*/

}

#endif
