//          Copyright Camille Gillot 2012 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_MMETHOD_MPL_HPP
#define RTTI_MMETHOD_MPL_HPP

#include "mmethod/config.hpp"

#include <boost/mpl/if.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>

#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>

#include <boost/mpl/pair.hpp>
#include <boost/mpl/single_view.hpp>
#include <boost/mpl/empty_sequence.hpp>

#include <boost/mpl/times.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/reverse_fold.hpp>

namespace boost {
namespace mpl {

template<>
struct clear_impl< empty_sequence::tag > {
  template<typename>
  struct apply {
    typedef empty_sequence type;
  };
};

} // namespace mpl
} // namespace boost

#endif
