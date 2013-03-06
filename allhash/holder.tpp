#ifndef RTTI_HOLDER_TPP
#define RTTI_HOLDER_TPP

#include "rtti_allhash/holder.hpp"
#include "rtti_allhash/getter.hpp"

#include "util/assert.hpp"

#include <boost/type_traits/is_const.hpp>

#include "rtti_allhash/holder.hpp"

namespace rtti {
namespace detail {

// an POD aggregate initialized with a constant expression
// standard ensures initialization before any non-trivial constructor called
template<class T, class Rt>
rtti_node holder<T,Rt>::node = {
  RTTI_GETTER::traits<T>::hash
, &get_holder<super>::type::node // constant pointer
};

template<class Rt>
const rtti_node holder<Rt,Rt>::node
  = { 0, 0 };

}} // namespace rtti::detail


#endif
