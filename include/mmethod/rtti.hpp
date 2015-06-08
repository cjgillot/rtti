//          Copyright Camille Gillot 2012 - 2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HPP
#define RTTI_HPP

#include "mmethod/config.hpp"
#include "mmethod/rtti/getter.hpp"
#include "mmethod/rtti/mixin.hpp"

/*! \mainpage RTTI library

  This library creates a efficient replacement of default C++ RTTI.

  Using a ISO C++ construct, RTTI provides access to a unique integer per class in the hierarchy.
  For each class, its bases' ids are also exposed as a linked list. This feature
  allows dynamic introspection through the hierarchy.

  For now, RTTI requires C++11.

  Note : ids are garanteed to be constructed when main() starts. Using those
    before entering main() results in undefined behaviour.

  See examples/ folder for examples using this library.

*/

#endif
