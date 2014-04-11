//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MMETHOD_TABLE_TABLE_HPP
#define BOOST_MMETHOD_TABLE_TABLE_HPP

#include "boost/mmethod/config.hpp"
#include "boost/mmethod/rttifwd.hpp"
#include "boost/mmethod/export/hash_map.hpp"

#include <boost/type_traits/is_volatile.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_class.hpp>

#include <stdint.h>

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace boost {
namespace mmethod {
namespace detail {

using boost::mmethod::invoker_t;
BOOST_STATIC_ASSERT_MSG( sizeof(invoker_t) <= sizeof(uintptr_t), "Platform not supported" );

//! useful typedefs
typedef hash_detail::hash_map poles_map_type;
typedef invoker_t* invoker_table_type;

//! structure holding tables
template<typename Tag>
struct register_base {
  BOOST_STATIC_ASSERT_MSG(
    boost::is_class<Tag>::value
  && !boost::is_const<Tag>::value
  && !boost::is_volatile<Tag>::value
  , "Invalid use of register_base<>"
  );

  template<std::size_t> struct poles {
    static detail::poles_map_type array;
  };

  static detail::invoker_table_type invoker_table;

};

template<typename Tag>
invoker_table_type register_base<Tag>::invoker_table;

template<typename Tag>
template<std::size_t J>
poles_map_type register_base<Tag>::poles<J>::array;

//! structure holding tables
template<typename Type>
struct get_register {
private:
  typedef typename boost::remove_cv<Type>::type Tag;
  typedef register_base<Tag> base;

public:
  template<std::size_t J>
  static BOOST_CONSTEXPR poles_map_type& poles() {
    return base::template poles<J>::array;
  }
  static BOOST_CONSTEXPR invoker_table_type& table() {
    return base::invoker_table;
  }
};

}}} // namespace boost::mmethod::detail

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif
