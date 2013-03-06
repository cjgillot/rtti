#ifndef RTTI_CREATOR_TPP
#define RTTI_CREATOR_TPP

#include "rtti/rttifwd.hpp"
#include "rtti/getter.hpp"

#include "rtti/creator.hpp"

namespace rtti {
namespace detail {

template<class Rt>
template<class T, bool has_static_id>
struct creator<Rt>::create_helper
{
  static inline rtti_type
  run()
  {
    // check for overflow
    ASSERT_MSG(
      creator<Rt>::current != 0
    , "Too many instanciated classes for current RTTI."
    );

    // scope lock a mutex if needed
    return creator<Rt>::current++;
  }

  enum { init = 0 };
};

template<class Rt>
template<class T>
struct creator<Rt>::create_helper<T, true>
{
  static inline rtti_type
  run()
  { return init; }

  enum {
    init = RTTI_GETTER::traits<T>::hash
  };
};

template<class Rt>
template<class T>
inline rtti_type
creator<Rt>::create()
{ return create_helper<T, RTTI_GETTER::traits<T>::static_>::run(); }

// initialize with 1, base class has static 0 id
template<class Rt>
rtti_type creator<Rt>::current = RTTI_GETTER::traits<Rt>::static_max + 1;

template<class Rt>
template<class T>
struct creator<Rt>::init_value
{
  enum {
    value = create_helper<T, RTTI_GETTER::traits<T>::static_>::init
  };
};

}} // namespace rtti::detail

#endif
