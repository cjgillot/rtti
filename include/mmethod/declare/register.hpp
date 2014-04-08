//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_DECLARE_REGISTER_HPP
#define BOOST_MMETHOD_DECLARE_REGISTER_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/declare/helper.hpp"

namespace boost {
namespace mmethod {

template<typename Tag, typename Sig>
struct mmethod_register
: detail::make_declare<Tag, Sig>::type {
private:
  typedef typename detail::make_declare<Tag, Sig>::type decl_maker;

public:
  typedef typename decl_maker::func_t function_type;
  using typename decl_maker::result_type;

  using decl_maker::operator();
  using decl_maker::fetch;
  using decl_maker::insert;
  using decl_maker::generate;
};

}} // namespace boost::mmethod

#define BOOST_MMETHOD_TAG(name) BOOST_JOIN(rtti_mmethod_tags__, name)

#define BOOST_MMETHOD_DECLARATION(tag, ret, sig)        \
struct tag                                              \
: boost::mmethod::mmethod_register<                     \
  tag                                                   \
, ret sig   > {                                         \
  template<typename> struct overload;                   \
} /* ; */

#define BOOST_MMETHOD_DECLARE(name, ret, sig)                   \
BOOST_MMETHOD_DECLARATION(BOOST_MMETHOD_TAG(name), ret, sig);   \
static BOOST_MMETHOD_TAG(name) name /* ; */

#endif
