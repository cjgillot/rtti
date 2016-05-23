<!--
          Copyright Camille Gillot 2012 - 2016.
 Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
-->

# MMethod

Implementation of multimethods in C++.

## Rtti library

This library is an efficient class hierarchy introspection,
working as a drop-in replacement to C++ ``` typeid ``` construct.

Using a ISO C++ construct, RTTI provides access to a unique integer per class in the hierarchy.
For each class, its bases' ids are also exposed.
This feature allows dynamic introspection of the hierarchy.

See *test/rtt/* folder for examples using this library.

## MMETHOD library

This library implements open multi-methods in ISO C++.

The external interface fits in two files :
- *include/mmethod/mmethod.hpp*
- *include/mmethod/implement.hpp*

*mmethod.hpp* exports everything needed to declare
and invoke mmethods. *mmethod.hpp*

*implement.hpp* exports everything needed to implement
formerly declared mmethods.

See *test/mmethod/* folder for examples using this library.

## Supported compilers

GCC 4.9 and CLANG 3.5 have been succesfully tested.
