#ifndef RTTI_MM_REGISTER_HPP
#define RTTI_MM_REGISTER_HPP

#include <cstdint>

#include <unordered_map>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comma.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/identity.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/fold_left.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

#include "rtti_allhash/getter.hpp"
#include "rtti_allhash/holder.hpp"
#include "rtti_allhash/multimethod/print.hpp"
#include "rtti_allhash/multimethod/poles.hpp"

namespace rtti {
namespace mmethod {

struct bad_dispatch
: std::exception
{
  const char* what() const noexcept
  { return "Unresolved call"; }
};

// type traits utilities
namespace detail {
template<typename T>
struct remove_all
{ typedef T type; };
template<typename T>
struct remove_all<T*>
: remove_all<T> {};
template<typename T>
struct remove_all<T&>
: remove_all<T> {};
template<typename T>
struct remove_all<T&&>
: remove_all<T> {};
template<typename T>
struct remove_all<const T>
: remove_all<T> {};
template<typename T>
struct remove_all<volatile T>
: remove_all<T> {};
}

typedef void(*invoker_t)();

// tag templates -> mark dispatch-guilty types (virtual) and the others
template<typename T>
struct virtual_tag
{
  typedef T type;
};

template<typename T>
struct static_tag
{
  typedef T type;
};

namespace detail {

// compute dispatch arity
template<typename... Types>
struct virtual_size;
template<typename First, typename... Types>
struct virtual_size<virtual_tag<First>, Types...>
{ enum { value = 1 + virtual_size<Types...>::value }; };
template<typename First, typename... Types>
struct virtual_size<static_tag<First>, Types...>
: virtual_size<Types...> {};
template<>
struct virtual_size<>
{ enum { value = 0 }; };

/// fetch_poles<>::eval(spec,_,args) loops over args and blits pole-data into [spec]
/// equivalent pseudo-code :
/// \code
/// i = 0
/// for each arg,
///   if arg is virtual
///     spec[i] = arg->pole->data
///     ++i
/// \endcode
/// The spec function is then used for dispatch computation
template<size_t I, size_t J, typename Tag, typename... Types>
struct fetch_poles;
template<size_t I, size_t J, typename Tag, typename First, typename... Types>
struct fetch_poles<I, J, Tag, virtual_tag<First>, Types...>
{
  template<std::size_t N, typename First2, typename... Types2>
  static void eval(std::size_t(&spec)[N], First2&& arg, Types2&&... args)
  {
    typedef typename Tag::template poles<J> poles_type;
    spec[I] = fetch_pole(
      poles_type::map
    , RTTI_NODE( std::forward<First2>(arg) )
    );
    fetch_poles<I+1, J+1, Tag, Types...>::eval(spec, std::forward<Types2&&>(args)...);
  }
};
template<size_t I, size_t J, typename Tag, typename First, typename... Types>
struct fetch_poles<I, J, Tag, static_tag<First>, Types...>
{
  template<std::size_t N, typename First2, typename... Types2>
  static void eval(std::size_t(&spec)[N], First2&&, Types2&&... args)
  {
    fetch_poles<I, J+1, Tag, Types...>::eval(spec, std::forward<Types2&&>(args)...);
  }
};
template<size_t I, size_t J, typename Tag>
struct fetch_poles<I, J, Tag>
{
  template<std::size_t N>
  static void eval(std::size_t(&)[N])
  {}
};

template<std::size_t Arity, typename Tag, typename... Types>
struct fetch_invoker
{
  template<typename... Types2>
  static invoker_t eval(Types2&&... args)
  {
    std::size_t spec [ Arity ];
    detail::fetch_poles<0, 0, Tag, Types...>::eval(spec, std::forward<Types2&&>(args)...);
    return Tag::fetch(spec);
  }
};
template<typename Tag, typename... Types>
struct fetch_invoker<1, Tag, Types...>
{
  template<typename... Types2>
  static invoker_t eval(Types2&&... args)
  {
    std::size_t spec [1];
    detail::fetch_poles<0, 0, Tag, Types...>::eval(spec, std::forward<Types2&&>(args)...);
    return (invoker_t)spec[0];
  }
};

} // namespace detail

/// main dispatch function
template<typename Tag, typename Ret, typename... Types>
struct dispatch_help
{
  template<typename... Types2>
  static Ret call(Types2&&... args)
  {
    // initialization stuff
    static bool is_initialized = false;
    if( UNLIKELY(!is_initialized) )
    {
      is_initialized = true;
      Tag::do_initialize();
    }

    // actual work
    enum { arity = detail::virtual_size<Types...>::value };

    invoker_t f = detail::fetch_invoker<arity, Tag, Types...>::eval(std::forward<Types2&&>(args)...);
    if(!f) throw bad_dispatch();

    typedef Ret(*func_t)(typename Types::type&...);
    return reinterpret_cast<func_t>(f)(args...);
  }
};

template<typename TAG, size_t ARITY, bool... TAGS>
struct mmethod_declare { enum { sz = 5 + sizeof...(TAGS) }; };

template<typename TAG, size_t ARITY, typename... HASH>
struct mmethod_implement { enum { sz = 5 + sizeof...(HASH) }; };

/*!\name Hash path construction
 *
 * These functions construct a list containing the hashes, used by the external tool.
 * \{
 */
//@{
///\brief List node
template<size_t H, typename Base>
struct mmethod_hpath {};
///\brief List end tag
struct mmethod_hpath_end {};

///\brief Hash value retrieval
template<typename T>
struct hash {
  typedef typename detail::remove_all<T>::type RawT;
  enum { value = RTTI_TYPE_HASH(RawT) };
};

/*!\brief List construction
 * Recursively constructs the list,
 * starting from \c T,
 * and upcasting,
 * until the base class.
 */
template<typename T>
struct mmethod_hpath_make {
  typedef typename detail::remove_all<T>::type RawT;
  typedef RTTI_GETTER::traits<RawT> Traits;
  typedef mmethod_hpath<
    Traits::hash
  , typename mmethod_hpath_make<typename Traits::super>::type
  > type;
};
/*!\brief List construction
 * This specialization is called by <tt>mmethod_hpath_make< </it>base class</it> ></tt>
 */
template<>
struct mmethod_hpath_make<const void> {
  typedef mmethod_hpath_end type;
};
//@}
/*!\}*/

template<bool>
struct invalid_node {
  static const rtti_node node;
};
template<bool B>
const rtti_node invalid_node<B>::node = { 0, 0 };

template<typename TAG, typename Ret, typename... Types>
struct trampoline {
  template<typename TAG2, typename Ret2, typename... Types2>
  struct apply {
    template<typename Over>
    static Ret call(Types& ...args)
    { return Over::call(static_cast<Types2>(args)...); }
  };
};

namespace tags {}

// utility
#define __MMETHOD_IDENTITY(...) __VA_ARGS__
#define __MMETHOD_IGNORE(...) /**/

///\brief Get mmethod tag name
#define __MMETHOD_TAG(name) BOOST_PP_CAT(__rtti_mmethod_tags__, name)

/*!\name Signature parsing macros
 *
 * Signature has the following syntax :
 * <tt>(arg1)(arg2)...</tt>
 * in which an argument has the form :
 * <tt>virtual(polymorphic class type)(argument name)</tt> if dispatch uses this type
 * <tt>static(any type)(argument name)</tt> if dispatch ignores this type
 */

#define __MMETHOD_DECL_REAL_SPEC_static(klass)  klass __MMETHOD_IDENTITY
#define __MMETHOD_DECL_REAL_SPEC_virtual(klass) klass __MMETHOD_IDENTITY
#define __MMETHOD_DECL_REAL_SPEC(r, d, i, elem) /**/,/**/BOOST_PP_CAT(__MMETHOD_DECL_REAL_SPEC_, elem)
#define __MMETHOD_DECL_REAL_HEAD_SPEC(elem) BOOST_PP_CAT(__MMETHOD_DECL_REAL_SPEC_, elem)
#define __MMETHOD_DECL_REAL(args) __MMETHOD_DECL_REAL_HEAD_SPEC(BOOST_PP_SEQ_HEAD(args)) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_DECL_REAL_SPEC,_,BOOST_PP_SEQ_TAIL(args))

#define __MMETHOD_DECL_SPEC_static(klass)  klass __MMETHOD_IGNORE
#define __MMETHOD_DECL_SPEC_virtual(klass) klass __MMETHOD_IGNORE
#define __MMETHOD_DECL_SPEC(r, d, i, elem) /**/,/**/BOOST_PP_CAT(__MMETHOD_DECL_SPEC_, elem) BOOST_PP_CAT(__arg, BOOST_PP_INC(i))
#define __MMETHOD_DECL_HEAD_SPEC(elem) BOOST_PP_CAT(__MMETHOD_DECL_SPEC_, elem) __arg0
#define __MMETHOD_DECL(args) __MMETHOD_DECL_HEAD_SPEC(BOOST_PP_SEQ_HEAD(args)) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_DECL_SPEC,_,BOOST_PP_SEQ_TAIL(args))

#define __MMETHOD_RAWTDECL_SPEC_static(klass)  klass __MMETHOD_IGNORE
#define __MMETHOD_RAWTDECL_SPEC_virtual __MMETHOD_RAWTDECL_SPEC_static
#define __MMETHOD_RAWTDECL_SPEC(r, d, i, elem) /**/,/**/ BOOST_PP_CAT(__MMETHOD_RAWTDECL_SPEC_, elem)
#define __MMETHOD_RAWTDECL_HEAD_SPEC(elem) BOOST_PP_CAT(__MMETHOD_RAWTDECL_SPEC_, elem)
#define __MMETHOD_RAWTDECL(args) __MMETHOD_RAWTDECL_HEAD_SPEC(BOOST_PP_SEQ_HEAD(args)) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_RAWTDECL_SPEC,_,BOOST_PP_SEQ_TAIL(args))

#define __MMETHOD_TDECL_SPEC_static(klass)  ::rtti::mmethod::static_tag <klass> __MMETHOD_IGNORE
#define __MMETHOD_TDECL_SPEC_virtual(klass) ::rtti::mmethod::virtual_tag<klass> __MMETHOD_IGNORE
#define __MMETHOD_TDECL_SPEC(r, d, i, elem) /**/,/**/ BOOST_PP_CAT(__MMETHOD_TDECL_SPEC_, elem)
#define __MMETHOD_TDECL_HEAD_SPEC(elem) BOOST_PP_CAT(__MMETHOD_TDECL_SPEC_, elem)
#define __MMETHOD_TDECL(args) __MMETHOD_TDECL_HEAD_SPEC(BOOST_PP_SEQ_HEAD(args)) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_TDECL_SPEC,_,BOOST_PP_SEQ_TAIL(args))

#define __MMETHOD_INFO_SPEC_static(klass)   ::rtti::mmethod::invalid_node<true>::node; __MMETHOD_IGNORE
#define __MMETHOD_INFO_SPEC_virtual(klass)  ::rtti::detail::get_holder<typename ::rtti::mmethod::detail::remove_all<klass>::type>::type::node; __MMETHOD_IGNORE
#define __MMETHOD_INFO_SPEC(r, path, i, elem)                                                   \
  template<> template<> ::rtti::rtti_node const& __MMETHOD_IDENTITY path::poles<i>::node =      \
    BOOST_PP_CAT(__MMETHOD_INFO_SPEC_, elem)
#define __MMETHOD_INFO(name, args) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_INFO_SPEC, (name), args)

#define __MMETHOD_HDECL_SPEC_static(klass)  __MMETHOD_IGNORE
#define __MMETHOD_HDECL_SPEC_virtual(klass) /**/,/**/ ::rtti::mmethod::hash<klass>::value __MMETHOD_IGNORE
#define __MMETHOD_HDECL_SPEC(r, d, i, elem) BOOST_PP_CAT(__MMETHOD_HDECL_SPEC_, elem)
#define __MMETHOD_HDECL_HEAD_SPEC(elem) BOOST_PP_CAT(__MMETHOD_HDECL_SPEC_, elem)
#define __MMETHOD_HDECL(args) 0 BOOST_PP_SEQ_FOR_EACH_I_R(5, __MMETHOD_HDECL_SPEC,_,args)

#define __MMETHOD_HASH_SPEC_static(klass) __MMETHOD_IGNORE
#define __MMETHOD_HASH_SPEC_virtual(klass) /**/,/**/ ::rtti::mmethod::mmethod_hpath_make<klass>::type __MMETHOD_IGNORE
#define __MMETHOD_HASH_SPEC(r, d, i, elem) BOOST_PP_CAT(__MMETHOD_HASH_SPEC_, elem)
#define __MMETHOD_HASH(ret, name, args) RTTI_PRINT( ::rtti::mmethod::mmethod_implement<__MMETHOD_TAG(name) BOOST_PP_COMMA() __MMETHOD_ARITY(args) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_HASH_SPEC,_,args) > );

#define __MMETHOD_SIGN_SPEC_static(klass)  /**/,/**/ false __MMETHOD_IGNORE
#define __MMETHOD_SIGN_SPEC_virtual(klass) /**/,/**/ true  __MMETHOD_IGNORE
#define __MMETHOD_SIGN_SPEC(r, d, i, elem) BOOST_PP_CAT(__MMETHOD_SIGN_SPEC_, elem)
#define __MMETHOD_SIGN(ret, name, args) RTTI_PRINT( ::rtti::mmethod::mmethod_declare<__MMETHOD_TAG(name) BOOST_PP_COMMA() __MMETHOD_ARITY(args) BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_SIGN_SPEC,_,args) > );

#define __MMETHOD_TARGS(ret, name, args)                        \
    __MMETHOD_TAG(name)                                         \
  , __MMETHOD_IDENTITY ret                                      \
  , __MMETHOD_TDECL(args)

#define __MMETHOD_RAWTARGS(ret, name, args)                     \
    __MMETHOD_TAG(name)                                         \
  , __MMETHOD_IDENTITY ret                                      \
  , __MMETHOD_RAWTDECL(args)

#define __MMETHOD_CALL_SPEC(r, d, i, elem) /**/,/**/ BOOST_PP_CAT(__arg, BOOST_PP_INC(i))
#define __MMETHOD_CALL(args) __arg0 BOOST_PP_SEQ_FOR_EACH_I(__MMETHOD_CALL_SPEC,_,BOOST_PP_SEQ_TAIL(args))

#define __MMETHOD_SPEC_virtual_M(arg) arg
#define __MMETHOD_SPEC_virtual(type) __MMETHOD_IDENTITY type __MMETHOD_SPEC_virtual_M

#define __MMETHOD_ARITY_SPEC_static(type) __MMETHOD_IDENTITY __MMETHOD_IGNORE
#define __MMETHOD_ARITY_SPEC_virtual(type) BOOST_PP_INC __MMETHOD_IGNORE
#define __MMETHOD_ARITY_SPEC(r,value,arg) BOOST_PP_CAT(__MMETHOD_ARITY_SPEC_, arg)(value)
#define __MMETHOD_ARITY(args) BOOST_PP_SEQ_FOLD_LEFT(__MMETHOD_ARITY_SPEC,0,args)

#define DECLARE_MMETHOD(ret, name, args)                        \
struct __MMETHOD_TAG(name) {                                    \
  __MMETHOD_SIGN(ret, name, args)                               \
  template<size_t... Types> struct overload;                    \
  typedef void(*invoker_t)();                                   \
  template<size_t> struct poles {                               \
    static ::rtti::mmethod::detail::map_type_base map;          \
  };                                                            \
  typedef ::rtti::mmethod::trampoline<__MMETHOD_RAWTARGS(ret, name, args)> trampoline; \
  static void do_initialize();                                  \
  static invoker_t fetch(std::size_t* spec);                    \
};                                                              \
inline __MMETHOD_IDENTITY ret                                   \
name(__MMETHOD_DECL(args)) {                                    \
  /* call here dispatch function */                             \
  return ret ::rtti::mmethod::dispatch_help<                    \
    __MMETHOD_TARGS(ret, name, args)                            \
  >::call(__MMETHOD_CALL(args));                                \
}

#define IMPLEMENT_MMETHOD(ret, name, args)                      \
template<>                                                      \
struct __MMETHOD_TAG(name)::overload<                           \
    __MMETHOD_HDECL(args)                                       \
> {                                                             \
  __MMETHOD_HASH(ret, name, args)                               \
  template<size_t> struct poles {                               \
    static ::rtti::rtti_node const& node;                       \
  };                                                            \
  typedef __MMETHOD_TAG(name)::trampoline::apply<__MMETHOD_RAWTARGS(ret,name,args)> trampoline;           \
  typedef void(*invoker_t)();                                   \
  static __MMETHOD_IDENTITY ret call(__MMETHOD_DECL(args));     \
  static const ::rtti::mmethod::invoker_t address;              \
};                                                              \
__MMETHOD_INFO(__MMETHOD_TAG(name)::overload<__MMETHOD_HDECL(args)>, args)\
::rtti::mmethod::invoker_t const                                \
__MMETHOD_TAG(name)::overload<__MMETHOD_HDECL(args)>            \
  ::address = (invoker_t)&trampoline::call<overload>;           \
__MMETHOD_IDENTITY ret                                          \
__MMETHOD_TAG(name)::overload<__MMETHOD_HDECL(args)>            \
  ::call(__MMETHOD_DECL_REAL(args))

}} // namespace rtti::mmethod

/// syntax sample
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

#endif
