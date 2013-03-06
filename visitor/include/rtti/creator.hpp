#ifndef RTTI_CREATOR_HPP
#define RTTI_CREATOR_HPP

#include "rtti/rttifwd.hpp"
#include "rtti/getter.hpp"

#include "util/assert.hpp"

#include <boost/type_traits/is_const.hpp>

namespace rtti {
namespace detail {

template<class Rt>
class creator {

  STATIC_ASSERT( boost::is_const<Rt>::value );

private:
  template<class, bool>
  struct create_helper;

public:
  template<class T>
  static rtti_type create();

  template<class T>
  struct init_value;

private:
  static rtti_type current;
};

}} // namespace rtti::detail

#endif
