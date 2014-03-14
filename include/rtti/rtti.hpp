//          Copyright Camille Gillot 2012 - 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef RTTI_HPP
#define RTTI_HPP

#include "rtti/holder/node.hpp"
#include "rtti/interface.hpp"
#include "rtti/mixin.hpp"

/*! \mainpage RTTI library

  This library creates a efficient replacement of default C++ RTTI.

  Using a ISO C++ construct, RTTI provides accès to a unique integer per class in the hierarchy.
  For each class, its bases' ids are also exposed as a linked list. This feature
  allows dynamic introspection through the hierarchy.
  
  For now, RTTI requires C++11.

  Note : ids are garanteed to be constructed when main() starts. Using those
    before entering main() results in undefined behaviour.
  
  See examples/ folder for examples using this library.

*/

#endif
