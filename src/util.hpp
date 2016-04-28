//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef UTIL_HPP
#define UTIL_HPP

#include "mmethod/config.hpp"

#include <algorithm>

namespace rtti_dispatch {

template<typename Cont>
void make_unique_sort(Cont& cont) {
  std::sort(cont.begin(), cont.end());
  cont.erase(std::unique(cont.begin(), cont.end()), cont.end());
}

} // namespace rtti_dispatch

#endif
